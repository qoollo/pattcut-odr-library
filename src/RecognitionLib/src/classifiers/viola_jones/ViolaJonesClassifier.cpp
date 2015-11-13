#include "inc/classifiers/viola_jones/ViolaJonesClassifier.h"
#include <math.h>
#include "inc/support/Rectangle.h"

int const ViolaJonesClassifier::_pixel_extent_by_scale = 2;
int const ViolaJonesClassifier::_max_image_size = 4096;

ViolaJonesClassifier::ViolaJonesClassifier()
{
	_wnd_width = 0;
	_wnd_height = 0;
	_scale_from = 0;
	_scale_to = 0;
	_scale_step = 0;
}

ViolaJonesClassifier::ViolaJonesClassifier(int wnd_width, int wnd_height, const CascadeClassifier& cascade_classifier, const StatMomentsClassifier& prefilter_classifier,
											ClassificationTag correct_tag, ClassificationTag incorrect_tag)
{
	assert(wnd_width > 0);
	assert(wnd_height > 0);
	assert(cascade_classifier.get_correct_tag() == correct_tag);

	_wnd_width = wnd_width;
	_wnd_height = wnd_height;
	_cascade_classfifer = cascade_classifier;
	_prefilter_classifier = prefilter_classifier;

	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;

	_scale_from = 0;
	_scale_to = 0;
	_scale_step = 0;

	update_statistic_data();
	update_scale_ranges();
}

ViolaJonesClassifier::ViolaJonesClassifier(const ViolaJonesClassifier& other)
{
	_wnd_width = other._wnd_width;
	_wnd_height = other._wnd_height;
	_cascade_classfifer = other._cascade_classfifer;
	_prefilter_classifier = other._prefilter_classifier;

	_correct_tag = other._correct_tag;
	_incorrect_tag = other._incorrect_tag;

	_scale_from = other._scale_from;
	_scale_to = other._scale_to;
	_scale_step = other._scale_step;

	_scaled_cascade = other._scaled_cascade;
	_cumulative_weak_cls_by_stage = other._cumulative_weak_cls_by_stage;
}
ViolaJonesClassifier::ViolaJonesClassifier(ViolaJonesClassifier&& other)
{
	_wnd_width = other._wnd_width;
	_wnd_height = other._wnd_height;
	_cascade_classfifer = std::move(other._cascade_classfifer);
	_prefilter_classifier = std::move(other._prefilter_classifier);

	_correct_tag = other._correct_tag;
	_incorrect_tag = other._incorrect_tag;

	_scale_from = other._scale_from;
	_scale_to = other._scale_to;
	_scale_step = other._scale_step;

	_scaled_cascade = std::move(other._scaled_cascade);
	_cumulative_weak_cls_by_stage = std::move(other._cumulative_weak_cls_by_stage);
}
ViolaJonesClassifier& ViolaJonesClassifier::operator=(const ViolaJonesClassifier& other)
{
	if (this == &other)
		return *this;

	_wnd_width = other._wnd_width;
	_wnd_height = other._wnd_height;
	_cascade_classfifer = other._cascade_classfifer;
	_prefilter_classifier = other._prefilter_classifier;

	_correct_tag = other._correct_tag;
	_incorrect_tag = other._incorrect_tag;

	_scale_from = other._scale_from;
	_scale_to = other._scale_to;
	_scale_step = other._scale_step;

	_scaled_cascade = other._scaled_cascade;
	_cumulative_weak_cls_by_stage = other._cumulative_weak_cls_by_stage;

	return *this;
}
ViolaJonesClassifier& ViolaJonesClassifier::operator=(ViolaJonesClassifier&& other)
{
	if (this == &other)
		return *this;

	_wnd_width = other._wnd_width;
	_wnd_height = other._wnd_height;
	_cascade_classfifer = std::move(other._cascade_classfifer);
	_prefilter_classifier = std::move(other._prefilter_classifier);

	_correct_tag = other._correct_tag;
	_incorrect_tag = other._incorrect_tag;

	_scale_from = other._scale_from;
	_scale_to = other._scale_to;
	_scale_step = other._scale_step;

	_scaled_cascade = std::move(other._scaled_cascade);
	_cumulative_weak_cls_by_stage = std::move(other._cumulative_weak_cls_by_stage);

	return *this;
}

void ViolaJonesClassifier::update_statistic_data()
{
	std::vector<int> stat_data(_cascade_classfifer.get_classifier_count());
	int prev_val = 0;

	for (unsigned int i = 0; i < stat_data.size(); i++)
	{
		auto& elem = _cascade_classfifer.get_classifier_at(i);
		stat_data[i] = prev_val + (int)elem.get_classifier_count();
		prev_val = stat_data[i];
	}

	_cumulative_weak_cls_by_stage = std::move(stat_data);
}

void ViolaJonesClassifier::update_scale_ranges(double from, double to, double step)
{
	if (step <= 0)
	{
		auto max_dim = fmax(_wnd_width, _wnd_height);
		step = (double)max_dim / (max_dim + 0.5);
	}

	if (from <= 0)
		from = 0.03 + 1.0 / step;

	if (to <= 0)
	{
		auto max_dim = fmax(_wnd_width, _wnd_height);
		to = (double)max_dim / _max_image_size + 0.0000001;
	}

	if (from < to)
	{
		double temp = from;
		from = to;
		to = temp;
	}

	std::vector<ScaledCascadeInfoCompact> scaled_data;
	scaled_data.reserve((int)(std::log(to / from) / std::log(step)) + 5);

	for (double cur_scale = from; cur_scale >= to * step; cur_scale *= step)
	{
		double wnd_step = fmax(1.0, 1.0 / cur_scale);
		try
		{
			scaled_data.push_back(ScaledCascadeInfoCompact(_cascade_classfifer.create_compact(1.0 / cur_scale),
				(int)round(_wnd_width / cur_scale),
				(int)round(_wnd_height / cur_scale),
				cur_scale, wnd_step));
		}
		catch (InvalidScaleException&)
		{
			// skip invalid scale
		}
	}

	_scale_from = from;
	_scale_to = to;
	_scale_step = step;
	_scaled_cascade = std::move(scaled_data);

}

void ViolaJonesClassifier::scale_correction(const IntegralImage& image, int min_object_size, int max_object_size, double scale_factor, double &scale_from_out, double &scale_to_out, double &scale_step_out) const
{
	double max_wnd_dim = fmax(_wnd_width, _wnd_height);

	if (scale_factor <= 1.0)
		scale_step_out = (double)max_wnd_dim / (max_wnd_dim + _pixel_extent_by_scale);
	else
		scale_step_out = 1.0 / scale_factor;

	if (min_object_size <= 0)
		scale_from_out = _scaled_cascade[0].get_scale_factor() + 0.0000001;
	else
		scale_from_out = max_wnd_dim / min_object_size;

	if (max_object_size <= 0)
		scale_to_out = _scaled_cascade[_scaled_cascade.size() - 1].get_scale_factor() - 0.000000001;
	else
		scale_to_out = max_wnd_dim / max_object_size;


	auto min_good_scale = fmax((double)_wnd_width / image.get_width(), (double)_wnd_height / image.get_height()) + 0.000000001;

	if (scale_from_out < min_good_scale)
		scale_from_out = min_good_scale;

	if (scale_to_out < min_good_scale)
		scale_to_out = min_good_scale;

	if (scale_from_out < scale_to_out)
	{
		double temp = scale_from_out;
		scale_from_out = scale_to_out;
		scale_to_out = temp;
	}
}



std::vector<Rectangle> ViolaJonesClassifier::apply(const IntegralImage& image, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
{
	std::vector<Rectangle> res;

	double scale_from, scale_to, scale_step;
	scale_correction(image, min_object_size, max_object_size, scale_factor, scale_from, scale_to, scale_step);

	if (step_factor <= 0)
		step_factor = 1.0;

	WindowIntegralImage wnd_integ_img(&image);

	unsigned int cur_cascade_scale_id = 0;
	double cur_cascade_scale = scale_from;

	while (cur_cascade_scale_id < _scaled_cascade.size() && _scaled_cascade[cur_cascade_scale_id].get_scale_factor() > cur_cascade_scale)
		cur_cascade_scale_id++;

	while (cur_cascade_scale_id < (_scaled_cascade.size() - 1) && _scaled_cascade[cur_cascade_scale_id].get_scale_factor() >= scale_to)
	{
		if (fabs(_scaled_cascade[cur_cascade_scale_id].get_scale_factor() - cur_cascade_scale) > fabs(_scaled_cascade[cur_cascade_scale_id + 1].get_scale_factor() - cur_cascade_scale))
		{
			cur_cascade_scale_id++;
			continue;
		}

		auto& cur_elem = _scaled_cascade[cur_cascade_scale_id];
		wnd_integ_img.set_window_params(0, 0, cur_elem.get_wnd_width(), cur_elem.get_wnd_height());
		auto max_x_pos = wnd_integ_img.get_max_window_x_position();
		auto max_y_pos = wnd_integ_img.get_max_window_y_position();
		auto step = cur_elem.get_step() * step_factor;

		for (double y = 0; y < max_y_pos; y += step)
		{
			for (double x = 0; x < max_x_pos; x += step)
			{
				wnd_integ_img.set_window_position((int)x, (int)y);

				if (_prefilter_classifier.evaluate_tag(wnd_integ_img) == _correct_tag)
				{
					if (cur_elem.get_cascade_classifier().evaluate_tag(wnd_integ_img) == _correct_tag)
						res.push_back(Rectangle(wnd_integ_img.get_x_position(), wnd_integ_img.get_y_position(),
						wnd_integ_img.get_width(), wnd_integ_img.get_height()));
				}
				else
				{
					x += step;
				}
			}
		}
		cur_cascade_scale_id++;
		cur_cascade_scale *= scale_step;
	}

	return std::move(res);
}

ViolaJonesOutputWithStatistic ViolaJonesClassifier::apply_with_statistic(const IntegralImage& image, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
{
	std::vector<AreaWithExtInfo> res;
	int proc_wnd_count = 0;
	int wnd_after_prefilter = 0;
	int total_weak_count = 0;
	int total_depth = 0;

	double scale_from, scale_to, scale_step;
	scale_correction(image, min_object_size, max_object_size, scale_factor, scale_from, scale_to, scale_step);

	if (step_factor <= 0)
		step_factor = 1.0;

	WindowIntegralImage wnd_integ_img(&image);

	unsigned int cur_cascade_scale_id = 0;
	double cur_cascade_scale = scale_from;

	while (cur_cascade_scale_id < _scaled_cascade.size() && _scaled_cascade[cur_cascade_scale_id].get_scale_factor() > cur_cascade_scale)
		cur_cascade_scale_id++;

	while (cur_cascade_scale_id < (_scaled_cascade.size() - 1) && _scaled_cascade[cur_cascade_scale_id].get_scale_factor() >= scale_to)
	{
		if (fabs(_scaled_cascade[cur_cascade_scale_id].get_scale_factor() - cur_cascade_scale) > fabs(_scaled_cascade[cur_cascade_scale_id + 1].get_scale_factor() - cur_cascade_scale))
		{
			cur_cascade_scale_id++;
			continue;
		}

		auto& cur_elem = _scaled_cascade[cur_cascade_scale_id];
		wnd_integ_img.set_window_params(0, 0, cur_elem.get_wnd_width(), cur_elem.get_wnd_height());
		auto max_x_pos = wnd_integ_img.get_max_window_x_position();
		auto max_y_pos = wnd_integ_img.get_max_window_y_position();
		auto step = cur_elem.get_step() * step_factor;
		auto fixed_prefilter = _prefilter_classifier.create_fixed_for_area(cur_elem.get_wnd_width(), cur_elem.get_wnd_height());

		for (double y = 0; y < max_y_pos; y += step)
		{
			for (double x = 0; x < max_x_pos; x += step)
			{
				proc_wnd_count++;
				auto next_window_info = wnd_integ_img.get_window_data((int)x, (int)y);

				if (fixed_prefilter.evaluate_bool(next_window_info))
				{
					wnd_after_prefilter++;
					wnd_integ_img.set_window_position(next_window_info);

					auto local_stat = cur_elem.get_cascade_classifier().evaluate_with_statistic(wnd_integ_img);
					total_weak_count += _cumulative_weak_cls_by_stage[local_stat.get_depth() - 1];
					total_depth += local_stat.get_depth();

					if (local_stat.get_classification_result().tag() == _correct_tag)
					{
						double comb_output_value = local_stat.get_combined_clasification_value() / _cumulative_weak_cls_by_stage[local_stat.get_depth() - 1];

						auto last_boosted_classifier_value = local_stat.get_classification_result();

						res.push_back(AreaWithExtInfo(Rectangle(wnd_integ_img.get_x_position(), wnd_integ_img.get_y_position(),
							wnd_integ_img.get_width(), wnd_integ_img.get_height()), last_boosted_classifier_value, comb_output_value));

					}
					else if (local_stat.get_depth() < 2)
					{
						x += step;
					}
				}
				else
				{
					x += step;
				}
			}
		}
		cur_cascade_scale_id++;
		cur_cascade_scale *= scale_step;
	}

	if (wnd_after_prefilter == 0)
		return ViolaJonesOutputWithStatistic(std::move(res), proc_wnd_count, 0, 0, wnd_after_prefilter);

	return ViolaJonesOutputWithStatistic(std::move(res), proc_wnd_count, (double)total_weak_count / wnd_after_prefilter, (double)total_depth / wnd_after_prefilter, wnd_after_prefilter);
}
