#include "inc/post_filtration/StandardResultFiltration.h"
#include <limits>
#include <vector>
#include <algorithm>

const double StandardResultFiltration::_intersect_square_proportion = 0.5;
const double StandardResultFiltration::_inclusion_square_proportion = 0.9;
const double StandardResultFiltration::_outer_area_priority = 1.2;
const int StandardResultFiltration::_first_pass_filtered_out_threshold = 2;
const int StandardResultFiltration::_second_pass_filtered_out_threshold = 3;

std::vector<FilteredAreaInfo> StandardResultFiltration::filter(const std::vector<Rectangle>& data) const
{
	std::vector<ResDataCombination> cur_cmb;
	std::vector<ResDataCombination> cur_out;
	cur_cmb.reserve(data.size());
	cur_out.reserve(data.size());

	for (unsigned int i = 0; i < data.size(); i++)
	{
		cur_cmb.push_back(ResDataCombination::from_rectangle(data[i]));
	}

	post_proccess(cur_cmb, cur_out);
	cur_cmb.clear();
	for (unsigned int i = 0; i < cur_out.size(); i++)
	{
		if (cur_out[i].get_count() >= StandardResultFiltration::_first_pass_filtered_out_threshold)
			cur_cmb.push_back(cur_out[i]);
	}
	std::sort(cur_cmb.begin(), cur_cmb.end(), [](const ResDataCombination& a, const ResDataCombination& b)
	{
		return (a.get_average_area().area() > b.get_average_area().area());
	});

	cur_out.clear();
	remove_inclusion(cur_cmb, cur_out);

	std::vector<FilteredAreaInfo> res;
	for (unsigned int i = 0; i < cur_out.size(); i++)
	{
		if (cur_out[i].get_count() >= StandardResultFiltration::_second_pass_filtered_out_threshold)
		{
			res.push_back(FilteredAreaInfo(cur_out[i].get_average_area(), cur_out[i].get_count()));
		}
	}

	return res;
}

void StandardResultFiltration::post_proccess(const std::vector<ResDataCombination>& src, std::vector<ResDataCombination>& res) const
{
	for (auto& src_area: src)
	{
		bool intersection_found = false;

		for (auto& res_area: res)
		{
			Rectangle rect_inter = Rectangle::intersect(src_area.get_average_area(), res_area.get_bound_area());
			auto square = rect_inter.area();
			if (square >= 1 &&
				square > _intersect_square_proportion * src_area.get_average_area().area() &&
				square > _intersect_square_proportion * res_area.get_bound_area().area())
			{
				res_area.include_area_proportional(src_area, (src_area.get_comb_boosted_classifier_value_max() > 0 ? src_area.get_comb_boosted_classifier_value_max() : 1.0));
				intersection_found = true;
				break;
			}
		}
		if (intersection_found == false)
			res.push_back(src_area);
	}

	for (auto& res_area: res)
		res_area.update_average();
}

void StandardResultFiltration::remove_inclusion(const std::vector<ResDataCombination>& src, std::vector<ResDataCombination>& res) const
{
	for (auto& src_area: src)
	{
		bool inclusion_found = false;

		for (auto& res_area: res)
		{
			Rectangle rect_inter = Rectangle::intersect(src_area.get_average_area(), res_area.get_average_area());
			auto square = rect_inter.area();
			if (square >= 1 &&
				(square > _inclusion_square_proportion * src_area.get_average_area().area() ||
				square > _inclusion_square_proportion * res_area.get_average_area().area()))
			{
				if (src_area.get_average_area().area() > res_area.get_average_area().area())
					res_area.include_area_proportional(src_area, _outer_area_priority);
				else
					res_area.include_area_proportional(src_area, 1.0 / _outer_area_priority);

				inclusion_found = true;
				break;
			}
		}
		if (inclusion_found == false)
			res.push_back(src_area);
	}

	for (auto& res_area: res)
		res_area.update_average();
}

ViolaJonesFilteredOutputWithStatistic StandardResultFiltration::filter(const ViolaJonesOutputWithStatistic& data) const
{
	std::vector<ResDataCombination> cur_cmb;
	std::vector<ResDataCombination> cur_out;
	cur_cmb.reserve(data.areas().size());
	cur_out.reserve(data.areas().size());

	for (unsigned int i = 0; i < data.areas().size(); i++)
	{
		auto temp = ResDataCombination::from_area_with_ext_info(data.areas()[i]);

		temp.set_aggregated_count(temp.get_aggregated_count() * temp.get_comb_boosted_classifier_value_max());
		temp.get_aggregated_area().left() *= temp.get_comb_boosted_classifier_value_max();
		temp.get_aggregated_area().top() *= temp.get_comb_boosted_classifier_value_max();
		temp.get_aggregated_area().width() *= temp.get_comb_boosted_classifier_value_max();
		temp.get_aggregated_area().height() *= temp.get_comb_boosted_classifier_value_max();

		cur_cmb.push_back(temp);
	}

	post_proccess(cur_cmb, cur_out);

	cur_cmb.clear();
	for (unsigned int i = 0; i < cur_out.size(); i++)
	{
		if (cur_out[i].get_count() >= StandardResultFiltration::_first_pass_filtered_out_threshold)
			cur_cmb.push_back(cur_out[i]);
	}
	std::sort(cur_cmb.begin(), cur_cmb.end(), [](const ResDataCombination& a, const ResDataCombination& b)
	{
		return (a.get_average_area().area() > b.get_average_area().area());
	});

	cur_out.clear();
	remove_inclusion(cur_cmb, cur_out);

	std::vector<FilteredAreaWithExtInfo> res_vect;
	for (unsigned int i = 0; i < cur_out.size(); i++)
	{
		if (cur_out[i].get_count() >= StandardResultFiltration::_second_pass_filtered_out_threshold)
			res_vect.push_back(FilteredAreaWithExtInfo(cur_out[i].get_average_area(), cur_out[i].get_count(),
			cur_out[i].get_last_boosted_classifier_value_min(), cur_out[i].get_last_boosted_classifier_value_max(), cur_out[i].get_last_boosted_classifier_value_aggregation() / cur_out[i].get_count(),
			cur_out[i].get_comb_boosted_classifier_value_min(), cur_out[i].get_comb_boosted_classifier_value_max(), cur_out[i].get_comb_boosted_classifier_value_aggregation() / cur_out[i].get_count()));
	}

	return ViolaJonesFilteredOutputWithStatistic(std::move(res_vect), data.get_wnd_count(), data.get_mean_weak_count(), data.get_mean_depth(), data.get_wnd_after_prefilter());
}