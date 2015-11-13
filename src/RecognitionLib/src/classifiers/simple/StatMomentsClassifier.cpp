#include "inc/classifiers/simple/StatMomentsClassifier.h"
#include <limits>


static float double_to_float(double src)
{
	if (src > (double)std::numeric_limits<float>().max())
		return std::numeric_limits<float>().max();
	if (src < (double)std::numeric_limits<float>().lowest())
		return std::numeric_limits<float>().lowest();

	return (float)src;
}

StatMomentsClassifier::StatMomentsClassifier()
{
	_mean_max = std::numeric_limits<float>::max();
	_mean_min = std::numeric_limits<float>::lowest();
	_var_max = std::numeric_limits<float>::max();
	_var_min = std::numeric_limits<float>::lowest();
}
StatMomentsClassifier::StatMomentsClassifier(double mean_min, double mean_max, double var_min, double var_max, ClassificationTag correct_tag, ClassificationTag incorrect_tag)
{
	_mean_min = double_to_float(mean_min);
	_mean_max = double_to_float(mean_max);
	_var_max = double_to_float(var_max);
	_var_min = double_to_float(var_min);
	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;
}
StatMomentsClassifier::StatMomentsClassifier(float mean_min, float mean_max, float var_min, float var_max, ClassificationTag correct_tag, ClassificationTag incorrect_tag)
{
	_mean_min = mean_min;
	_mean_max = mean_max;
	_var_max = var_max;
	_var_min = var_min;
	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;
}


FixedAreaStatMomentsClassifier StatMomentsClassifier::create_fixed_for_area(int width, int height) const
{
	assert(width > 0);
	assert(height > 0);

	uint32_t area = uint32_t(width * height);

	uint32_t sum_min = 0;
	if (_mean_min >= UINT32_MAX / area)
		sum_min = UINT32_MAX;
	else if (_mean_min > 0)
		sum_min = (uint32_t)(_mean_min * area);

	uint32_t sum_max = 0;
	if (_mean_max >= UINT32_MAX / area)
		sum_max = UINT32_MAX;
	else if (_mean_max > 0)
		sum_max = (uint32_t)(_mean_max * area);

	uint64_t accum_var_min = 0;
	if (_var_min >= UINT32_MAX / area)
		accum_var_min = UINT64_MAX;
	else if (_var_min > 0)
		accum_var_min = (uint64_t)(_var_min * _var_min * area * area);

	uint64_t accum_var_max = 0;
	if (_var_max >= UINT32_MAX / area)
		accum_var_max = UINT64_MAX;
	else if (_var_max > 0)
		accum_var_max = (uint64_t)(_var_max * _var_max * area * area);

	return FixedAreaStatMomentsClassifier(width, height, sum_min, sum_max, accum_var_min, accum_var_max);
}



StatMomentsClassifier StatMomentsClassifier::copy_scaled(double ratio) const
{
	return StatMomentsClassifier(*this);
}
