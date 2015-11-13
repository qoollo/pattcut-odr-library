#pragma once
#include "inc/haar_features/HaarFeatureCompact.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/images/WindowIntegralImage.h"

class StumpClassifierTwoRectCompact
{
public:
	StumpClassifierTwoRectCompact();
	StumpClassifierTwoRectCompact(const HaarFeatureTwoRectCompact &feature, double threshold, double result_on_less, double result_on_greater);

	inline float evaluate_on_feature_value(float feature_value) const
	{
		return _signed_confidence[feature_value < _threshold];
	}
	inline float evaluate(const WindowIntegralImage &img) const
	{
		return evaluate_on_feature_value(_feature.evaluate(img));
	}

private:
	HaarFeatureTwoRectCompact _feature;
	float _signed_confidence[2];
	float _threshold;
};


class StumpClassifierThreeRectCompact
{
public:
	StumpClassifierThreeRectCompact();
	StumpClassifierThreeRectCompact(const HaarFeatureThreeRectCompact &feature, double threshold, double result_on_less, double result_on_greater);

	inline float evaluate_on_feature_value(float feature_value) const
	{
		return _signed_confidence[feature_value < _threshold];
	}
	inline float evaluate(const WindowIntegralImage &img) const
	{
		return evaluate_on_feature_value(_feature.evaluate(img));
	}

private:
	HaarFeatureThreeRectCompact _feature;
	float _signed_confidence[2];
	float _threshold;
};