#pragma once
#include <assert.h>
#include "inc/accuracy_estimation/AccuracyResults.h"
#include "inc/post_filtration/FilteredAreaInfo.h"
#include "inc/post_filtration/FilteredAreaWithExtInfo.h"

class SimpleAccuracyEstimator
{
public:
	static const double default_window_norm_coef;
	static const double default_good_window_threshold;

	static const double default_comb_norm_coef;
	static const double default_comd_threshold;

	SimpleAccuracyEstimator()
	{
		_wnd_norm_coef = default_window_norm_coef;
		_good_wnd_threshold = default_good_window_threshold;

		_comb_norm_coef = default_comb_norm_coef;
		_comb_threshold = default_comd_threshold;
	}

	SimpleAccuracyEstimator(double window_norm_coef, double good_wnd_threshold)
	{
		assert(window_norm_coef > 0);

		_wnd_norm_coef = window_norm_coef;
		_good_wnd_threshold = good_wnd_threshold;

		_comb_norm_coef = default_comb_norm_coef;
		_comb_threshold = default_comd_threshold;
	}

	SimpleAccuracyEstimator(double window_norm_coef, double good_wnd_threshold, double comb_norm_coef, double comb_threshold)
	{
		assert(window_norm_coef > 0);
		assert(comb_norm_coef > 0);

		_wnd_norm_coef = window_norm_coef;
		_good_wnd_threshold = good_wnd_threshold;
		_comb_norm_coef = comb_norm_coef;
		_comb_threshold = comb_threshold;

	}

	AccuracyResults estimate(const FilteredAreaInfo& area_info) const
	{
		auto eval = (area_info.get_wnd_count() / _wnd_norm_coef) - _good_wnd_threshold;
		return AccuracyResults(eval);
	}

	AccuracyResults estimate(const FilteredAreaWithExtInfo& area_info) const
	{
		auto wnd_eval = (area_info.get_wnd_count() / _wnd_norm_coef) - _good_wnd_threshold;
		auto comb_eval = (area_info.get_comb_boosted_classifier_value_max() / _comb_norm_coef) - _comb_threshold;

		return AccuracyResults(wnd_eval + comb_eval);
	}

private:
	double _wnd_norm_coef;
	double _good_wnd_threshold;

	double _comb_norm_coef;
	double _comb_threshold;

};