#include "inc/haar_features/HaarFeature.h"
 
HaarFeature::HaarFeature()
{
	_is_three_rect = false;
}

HaarFeature::HaarFeature(const EvaluateRactangle& r1, const EvaluateRactangle& r2)
{
	_r1 = r1;
	_r2 = r2;
	_is_three_rect = false;
}

HaarFeature::HaarFeature(const EvaluateRactangle& r1, const EvaluateRactangle& r2, const EvaluateRactangle& r3)
{
	_r1 = r1;
	_r2 = r2;
	_r3 = r3;
	_is_three_rect = true;
}

double HaarFeature::evaluate(const WindowIntegralImage &img) const
{
	double r1_eval = _r1.get_corrected_weight() * img.get_sum_in_rectangle(_r1.get_x1(), _r1.get_y1(), _r1.get_x2(), _r1.get_y2());
	double r2_eval = _r2.get_corrected_weight() * img.get_sum_in_rectangle(_r2.get_x1(), _r2.get_y1(), _r2.get_x2(), _r2.get_y2());

	if (!_is_three_rect)
		return (r1_eval + r2_eval) * img.get_inv_variance();

	double r3_eval = _r3.get_corrected_weight() * img.get_sum_in_rectangle(_r3.get_x1(), _r3.get_y1(), _r3.get_x2(), _r3.get_y2());
	return (r1_eval + r2_eval + r3_eval) * img.get_inv_variance();
}

double HaarFeature::evaluate_two_rect(const WindowIntegralImage &img) const
{
	assert(_is_three_rect == false);

	double r1_eval = _r1.get_corrected_weight() * img.get_sum_in_rectangle(_r1.get_x1(), _r1.get_y1(), _r1.get_x2(), _r1.get_y2());
	double r2_eval = _r2.get_corrected_weight() * img.get_sum_in_rectangle(_r2.get_x1(), _r2.get_y1(), _r2.get_x2(), _r2.get_y2());

	return (r1_eval + r2_eval) * img.get_inv_variance();
}

double HaarFeature::evaluate_three_rect(const WindowIntegralImage &img) const
{
	assert(_is_three_rect == true);

	double r1_eval = _r1.get_corrected_weight() * img.get_sum_in_rectangle(_r1.get_x1(), _r1.get_y1(), _r1.get_x2(), _r1.get_y2());
	double r2_eval = _r2.get_corrected_weight() * img.get_sum_in_rectangle(_r2.get_x1(), _r2.get_y1(), _r2.get_x2(), _r2.get_y2());
	double r3_eval = _r3.get_corrected_weight() * img.get_sum_in_rectangle(_r3.get_x1(), _r3.get_y1(), _r3.get_x2(), _r3.get_y2());

	return (r1_eval + r2_eval + r3_eval) * img.get_inv_variance();
}

HaarFeature HaarFeature::copy_scaled(double ratio)
{
	if (_is_three_rect)
		return HaarFeature(_r1.copy_scaled(ratio), _r2.copy_scaled(ratio), _r3.copy_scaled(ratio));
	else
		return HaarFeature(_r1.copy_scaled(ratio), _r2.copy_scaled(ratio));
}

HaarFeatureTwoRectCompact HaarFeature::create_compact_two_rect(double ratio)
{
	assert(_is_three_rect == false);
	return HaarFeatureTwoRectCompact(_r1.create_compact(ratio), _r2.create_compact(ratio));
}

HaarFeatureThreeRectCompact HaarFeature::create_compact_three_rect(double ratio)
{
	assert(_is_three_rect == true);
	return HaarFeatureThreeRectCompact(_r1.create_compact(ratio), _r2.create_compact(ratio), _r3.create_compact(ratio));
}