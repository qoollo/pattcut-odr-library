#include "inc/classifiers/simple/StumpClassifierCompact.h"
#include <limits>

static float double_to_float(double src)
{
	if (src > (double)std::numeric_limits<float>().max())
		return std::numeric_limits<float>().max();
	if (src < (double)std::numeric_limits<float>().lowest())
		return std::numeric_limits<float>().lowest();

	return (float)src;
}


StumpClassifierTwoRectCompact::StumpClassifierTwoRectCompact()
{
	_threshold = 0.0f;

	_signed_confidence[0] = 0.0f;
	_signed_confidence[1] = 0.0f;
}


StumpClassifierTwoRectCompact::StumpClassifierTwoRectCompact(const HaarFeatureTwoRectCompact &feature, double threshold, double result_on_less, double result_on_greater)
{
	_feature = feature;
	_threshold = double_to_float(threshold);

	_signed_confidence[0] = double_to_float(result_on_greater);
	_signed_confidence[1] = double_to_float(result_on_less);
}



StumpClassifierThreeRectCompact::StumpClassifierThreeRectCompact()
{
	_threshold = 0.0f;

	_signed_confidence[0] = 0.0f;
	_signed_confidence[1] = 0.0f;
}

StumpClassifierThreeRectCompact::StumpClassifierThreeRectCompact(const HaarFeatureThreeRectCompact &feature, double threshold, double result_on_less, double result_on_greater)
{
	_feature = feature;
	_threshold = double_to_float(threshold);

	_signed_confidence[0] = double_to_float(result_on_greater);
	_signed_confidence[1] = double_to_float(result_on_less);
}