#include "inc/accuracy_estimation/LogisticAccuracyEstimator.h"
#include <math.h>

double const LogisticAccuracyEstimator::default_window_norm_coef = 27.0;
double const LogisticAccuracyEstimator::default_comb_norm_coef = 100.0;
double const LogisticAccuracyEstimator::default_threshold = 0.85;
double const LogisticAccuracyEstimator::default_theta[6] = { -4.064696587590060,
															  3.900390327725805,
															  9.115724644925429,
															  0.036809800400294,
															  3.841244686735187,
															  3.512271427991473 };


LogisticAccuracyEstimator::LogisticAccuracyEstimator()
{
	for (int i = 0; i < 6; i++)
		_theta[i] = LogisticAccuracyEstimator::default_theta[i];

	_wnd_norm_coef = LogisticAccuracyEstimator::default_window_norm_coef;
	_comb_norm_coef = LogisticAccuracyEstimator::default_window_norm_coef;
	_threshold = LogisticAccuracyEstimator::default_threshold;
}

LogisticAccuracyEstimator::LogisticAccuracyEstimator(double window_norm_coef, double comb_norm_coef, double threshold)
{
	assert(window_norm_coef > 0);
	assert(comb_norm_coef > 0);

	for (int i = 0; i < 6; i++)
		_theta[i] = LogisticAccuracyEstimator::default_theta[i];

	_wnd_norm_coef = window_norm_coef;
	_comb_norm_coef = comb_norm_coef;
	_threshold = threshold;
}

LogisticAccuracyEstimator::LogisticAccuracyEstimator(double window_norm_coef, double comb_norm_coef, double threshold, double theta[6])
{
	assert(window_norm_coef > 0);
	assert(comb_norm_coef > 0);

	for (int i = 0; i < 6; i++)
		_theta[i] = theta[i];

	_wnd_norm_coef = window_norm_coef;
	_comb_norm_coef = comb_norm_coef;
	_threshold = threshold;

}


AccuracyResults LogisticAccuracyEstimator::estimate(const FilteredAreaWithExtInfo& area_info) const
{
	double x1 = (double)area_info.get_wnd_count() / _wnd_norm_coef;
	double x2 = (double)area_info.get_comb_boosted_classifier_value_max() / _comb_norm_coef;

	double reg_value = _theta[0] + x1 * _theta[1] + x2 * _theta[2] + x1 * x1 * _theta[3] + x2 * x2 * _theta[4] + x1 * x2 * _theta[5];
	double sigmoid_value = 1.0 / (1.0 + exp(-reg_value));

	double accuracy = sigmoid_value - _threshold;
	return AccuracyResults(accuracy, accuracy >= 0);
}