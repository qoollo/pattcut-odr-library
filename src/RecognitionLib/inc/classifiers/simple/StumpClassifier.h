#pragma once
#include "inc/haar_features/HaarFeature.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/simple/StumpClassifierCompact.h"

class StumpClassifier
{
public:
	StumpClassifier();
	StumpClassifier(const HaarFeature &feature, double threshold, const ClassificationResult &result_on_less, const ClassificationResult &result_on_greater);

	inline const HaarFeature& get_feature() const
	{
		return _feature;
	}
	inline void set_feature(const HaarFeature &feature)
	{
		_feature = feature;
	}

	inline double get_threshold() const
	{
		return _threshold;
	}
	inline void set_threshlod(double threshold)
	{
		_threshold = threshold;
	}

	inline ClassificationResult get_result_on_less() const
	{
		return _result_on_less;
	}
	inline void set_result_on_less(const ClassificationResult &result_on_less)
	{
		_result_on_less = result_on_less;
	}

	inline ClassificationResult get_result_on_greater() const
	{
		return _result_on_greater;
	}
	inline void set_result_on_greater(const ClassificationResult &result_on_greater)
	{
		_result_on_greater = result_on_greater;
	}

	ClassificationResult evaluate_on_feature_value(double feature_value) const;
	ClassificationTag evaluate_tag_on_feature_value(double feature_value) const;
	double evaluate_signed_confidence_on_feature_value(double feature_value) const;

	inline ClassificationResult evaluate(const WindowIntegralImage &img) const
	{
		return evaluate_on_feature_value(_feature.evaluate(img));
	}
	inline ClassificationTag evaluate_tag(const WindowIntegralImage &img) const
	{
		return evaluate_tag_on_feature_value(_feature.evaluate(img));
	}
	inline double evaluate_signed_confidence(const WindowIntegralImage &img) const
	{
		return evaluate_signed_confidence_on_feature_value(_feature.evaluate(img));
	}
	inline double evaluate_signed_confidence_for_two_rect(const WindowIntegralImage &img) const
	{
		return evaluate_signed_confidence_on_feature_value(_feature.evaluate_two_rect(img));
	}
	inline double evaluate_signed_confidence_for_three_rect(const WindowIntegralImage &img) const
	{
		return evaluate_signed_confidence_on_feature_value(_feature.evaluate_three_rect(img));
	}

	void init_signed_confidence(ClassificationTag correct_tag);

	StumpClassifier copy_scaled(double ratio);
	StumpClassifierTwoRectCompact create_compact_two_rect(double ratio);
	StumpClassifierThreeRectCompact create_compact_three_rect(double ratio);

private:
	HaarFeature _feature;
	double _threshold;
	ClassificationResult _result_on_less;
	ClassificationResult _result_on_greater;

	bool _signed_confidence_inited;
	double _signed_confidence[2];
};