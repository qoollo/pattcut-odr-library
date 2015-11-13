#pragma once
#include "inc/support/Rectangle.h"
#include "inc/support/RectangleF.h"
#include "inc/classifiers/viola_jones/AreaWithExtInfo.h"


class ResDataCombination
{
public:
	ResDataCombination();
	ResDataCombination(const ResDataCombination& init_data);
	static ResDataCombination as_new_area(const ResDataCombination& init_data);
	static ResDataCombination from_rectangle(const Rectangle& init_data);
	static ResDataCombination from_area_with_ext_info(const AreaWithExtInfo& init_data);

	void include_area(const ResDataCombination &area);

	void include_area_proportional(const ResDataCombination& area, double priority);

	void update_average();

	inline const Rectangle& get_average_area() const
	{
		return _average_area;
	}
	inline RectangleF& get_aggregated_area()
	{
		return _aggregated_area;
	}
	inline Rectangle& get_bound_area()
	{
		return _bound_area;
	}
	inline int get_count() const
	{
		return _count;
	}
	inline double get_aggregated_count() const
	{
		return _aggregated_count;
	}
	inline void set_aggregated_count(double aggregated_count)
	{
		_aggregated_count = aggregated_count;
	}

	inline double get_last_boosted_classifier_value_max() const
	{
		return _last_boosted_classifier_value_max;
	}
	inline double get_last_boosted_classifier_value_min() const
	{
		return _last_boosted_classifier_value_min;
	}
	inline double get_last_boosted_classifier_value_aggregation() const
	{
		return _last_boosted_classifier_value_aggregation;
	}

	inline double get_comb_boosted_classifier_value_max() const
	{
		return _comb_boosted_classifier_value_max;
	}
	inline double get_comb_boosted_classifier_value_min() const
	{
		return _comb_boosted_classifier_value_min;
	}
	inline double get_comb_boosted_classifier_value_aggregation() const
	{
		return _comb_boosted_classifier_value_aggregation;
	}

private:
	Rectangle _average_area;
	RectangleF _aggregated_area;
	Rectangle _bound_area;
	int _count;
	double _aggregated_count;

	double _last_boosted_classifier_value_max;
	double _last_boosted_classifier_value_min;
	double _last_boosted_classifier_value_aggregation;

	double _comb_boosted_classifier_value_max;
	double _comb_boosted_classifier_value_min;
	double _comb_boosted_classifier_value_aggregation;
};