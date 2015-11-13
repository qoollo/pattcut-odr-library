#pragma once
#include "inc/support/Rectangle.h"
#include "inc/classifiers/viola_jones/ViolaJonesOutputWithStatistic.h"
#include "inc/post_filtration/FilteredAreaInfo.h"
#include "inc/post_filtration/FilteredAreaWithExtInfo.h"
#include "inc/post_filtration/ResDataCombination.h"
#include "inc/post_filtration/ViolaJonesFilteredOutputWithStatistic.h"

class StandardResultFiltration
{
public:
	std::vector<FilteredAreaInfo> filter(const std::vector<Rectangle>& data) const;

	ViolaJonesFilteredOutputWithStatistic filter(const ViolaJonesOutputWithStatistic& data) const;

	static const double _intersect_square_proportion;
	static const double _inclusion_square_proportion;
	static const double _outer_area_priority;
	static const int _first_pass_filtered_out_threshold;
	static const int _second_pass_filtered_out_threshold;

private:
	void post_proccess(const std::vector<ResDataCombination>& src, std::vector<ResDataCombination>& res) const;

	void remove_inclusion(const std::vector<ResDataCombination>& src, std::vector<ResDataCombination>& res) const;
};