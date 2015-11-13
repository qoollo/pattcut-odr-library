#pragma once
#include <assert.h>
#include "inc/support/Rectangle.h"

class FilteredAreaWithExtInfo
{
public:
	FilteredAreaWithExtInfo()
	{
		_last_boosted_classifier_value_max = 0;
		_last_boosted_classifier_value_min = 0;
		_last_boosted_classifier_value_mean = 0;

		_comb_boosted_classifier_value_max = 0;
		_comb_boosted_classifier_value_min = 0;
		_comb_boosted_classifier_value_mean = 0;

		_wnd_count = 0;
	}

	FilteredAreaWithExtInfo(const Rectangle& area, int wnd_count, 
		double last_boosted_classifier_value_min, double last_boosted_classifier_value_max, double last_boosted_classifier_value_mean,
		double comb_boosted_classifier_value_min, double comb_boosted_classifier_value_max, double comb_boosted_classifier_value_mean)
	{
		assert(wnd_count > 0);

		_area = area;

		_last_boosted_classifier_value_max = last_boosted_classifier_value_max;
		_last_boosted_classifier_value_min = last_boosted_classifier_value_min;
		_last_boosted_classifier_value_mean = last_boosted_classifier_value_mean;

		_comb_boosted_classifier_value_max = comb_boosted_classifier_value_max;
		_comb_boosted_classifier_value_min = comb_boosted_classifier_value_min;
		_comb_boosted_classifier_value_mean = comb_boosted_classifier_value_mean;

		_wnd_count = wnd_count;
	}

	inline const Rectangle& get_area() const
	{
		return _area;
	}

	inline double get_last_boosted_classifier_value_max() const
	{
		return _last_boosted_classifier_value_max;
	}
	inline double get_last_boosted_classifier_value_min() const
	{
		return _last_boosted_classifier_value_min;
	}
	inline double get_last_boosted_classifier_value_mean() const
	{
		return _last_boosted_classifier_value_mean;
	}

	inline double get_comb_boosted_classifier_value_max() const
	{
		return _comb_boosted_classifier_value_max;
	}
	inline double get_comb_boosted_classifier_value_min() const
	{
		return _comb_boosted_classifier_value_min;
	}
	inline double get_comb_boosted_classifier_value_mean() const
	{
		return _comb_boosted_classifier_value_mean;
	}

	inline int get_wnd_count() const
	{
		return _wnd_count;
	}

private:
	Rectangle _area;

	double _last_boosted_classifier_value_max;
	double _last_boosted_classifier_value_min;
	double _last_boosted_classifier_value_mean;

	double _comb_boosted_classifier_value_max;
	double _comb_boosted_classifier_value_min;
	double _comb_boosted_classifier_value_mean;

	int _wnd_count;
};