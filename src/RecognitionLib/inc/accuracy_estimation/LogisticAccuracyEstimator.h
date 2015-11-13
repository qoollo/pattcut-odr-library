#pragma once
#include <assert.h>
#include "inc/accuracy_estimation/AccuracyResults.h"
#include "inc/post_filtration/FilteredAreaInfo.h"
#include "inc/post_filtration/FilteredAreaWithExtInfo.h"

class LogisticAccuracyEstimator
{
public:
	static const double default_window_norm_coef;
	static const double default_comb_norm_coef;
	static const double default_threshold;
	static const double default_theta[6];

	LogisticAccuracyEstimator();
	LogisticAccuracyEstimator(double window_norm_coef, double comb_norm_coef, double threshold);
	LogisticAccuracyEstimator(double window_norm_coef, double comb_norm_coef, double threshold, double theta[6]);

	AccuracyResults estimate(const FilteredAreaWithExtInfo& area_info) const;

private:
	double _theta[6];
	double _wnd_norm_coef;
	double _comb_norm_coef;
	double _threshold;
};