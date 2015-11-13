#include "inc/classifiers/simple/StumpClassifier.h"

StumpClassifier::StumpClassifier()
{
	_threshold = 0.0;
	_result_on_less = ClassificationResult(ClassificationTag(1));
	_result_on_greater = ClassificationResult(ClassificationTag(0));

	_signed_confidence_inited = false;
	_signed_confidence[0] = 0.0;
	_signed_confidence[1] = 0.0;
}

StumpClassifier::StumpClassifier(const HaarFeature &feature, double threshold, const ClassificationResult &result_on_less, const ClassificationResult &result_on_greater)
{
	_feature = feature;
	_threshold = threshold;
	_result_on_less = result_on_less;
	_result_on_greater = result_on_greater;

	_signed_confidence_inited = false;
	_signed_confidence[0] = 0.0;
	_signed_confidence[1] = 0.0;
}

void StumpClassifier::init_signed_confidence(ClassificationTag correct_tag)
{
	if (_result_on_less.tag() == correct_tag)
	{
		_signed_confidence[1] = _result_on_less.confidence();
		_signed_confidence[0] = -_result_on_greater.confidence();
		_signed_confidence_inited = true;
	}
	else if (_result_on_greater.tag() == correct_tag)
	{
		_signed_confidence[1] = -_result_on_less.confidence();
		_signed_confidence[0] = _result_on_greater.confidence();
		_signed_confidence_inited = true;
	}
}

ClassificationResult StumpClassifier::evaluate_on_feature_value(double feature_value) const
{
	return (feature_value < _threshold) ? _result_on_less : _result_on_greater;
}
ClassificationTag StumpClassifier::evaluate_tag_on_feature_value(double feature_value) const
{
	return (feature_value < _threshold) ? _result_on_less.tag() : _result_on_greater.tag();
}

double StumpClassifier::evaluate_signed_confidence_on_feature_value(double feature_value) const
{
	assert(_signed_confidence_inited);
	return _signed_confidence[feature_value < _threshold];
}

StumpClassifier StumpClassifier::copy_scaled(double ratio)
{
	return StumpClassifier(_feature.copy_scaled(ratio), _threshold * ratio * ratio, _result_on_less, _result_on_greater);
}
StumpClassifierTwoRectCompact StumpClassifier::create_compact_two_rect(double ratio)
{
	assert(_signed_confidence_inited == true);

	return StumpClassifierTwoRectCompact(_feature.create_compact_two_rect(ratio), _threshold * ratio * ratio, _signed_confidence[1], _signed_confidence[0]);
}
StumpClassifierThreeRectCompact StumpClassifier::create_compact_three_rect(double ratio)
{
	assert(_signed_confidence_inited == true);

	return StumpClassifierThreeRectCompact(_feature.create_compact_three_rect(ratio), _threshold * ratio * ratio, _signed_confidence[1], _signed_confidence[0]);
}