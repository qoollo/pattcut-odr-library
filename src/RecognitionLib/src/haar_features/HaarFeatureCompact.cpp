#include "inc/haar_features/HaarFeatureCompact.h"
 
HaarFeatureTwoRectCompact::HaarFeatureTwoRectCompact()
{
}

HaarFeatureTwoRectCompact::HaarFeatureTwoRectCompact(const EvaluateRactangleCompact& r1, const EvaluateRactangleCompact& r2)
{
	_r1 = r1;
	_r2 = r2;
}

float HaarFeatureTwoRectCompact::evaluate(const WindowIntegralImage &img) const
{
	float r1_eval = _r1.get_corrected_weight() * img.get_sum_in_rectangle(_r1.get_x1(), _r1.get_y1(), _r1.get_x2(), _r1.get_y2());
	float r2_eval = _r2.get_corrected_weight() * img.get_sum_in_rectangle(_r2.get_x1(), _r2.get_y1(), _r2.get_x2(), _r2.get_y2());
	return (r1_eval + r2_eval) * img.get_inv_variance();
}




HaarFeatureThreeRectCompact::HaarFeatureThreeRectCompact()
{
}


HaarFeatureThreeRectCompact::HaarFeatureThreeRectCompact(const EvaluateRactangleCompact& r1, const EvaluateRactangleCompact& r2, const EvaluateRactangleCompact& r3)
{
	_r1 = r1;
	_r2 = r2;
	_r3 = r3;
}

float HaarFeatureThreeRectCompact::evaluate(const WindowIntegralImage &img) const
{
	float r1_eval = _r1.get_corrected_weight() * img.get_sum_in_rectangle(_r1.get_x1(), _r1.get_y1(), _r1.get_x2(), _r1.get_y2());
	float r2_eval = _r2.get_corrected_weight() * img.get_sum_in_rectangle(_r2.get_x1(), _r2.get_y1(), _r2.get_x2(), _r2.get_y2());
	float r3_eval = _r3.get_corrected_weight() * img.get_sum_in_rectangle(_r3.get_x1(), _r3.get_y1(), _r3.get_x2(), _r3.get_y2());
	return (r1_eval + r2_eval + r3_eval) * img.get_inv_variance();
}
