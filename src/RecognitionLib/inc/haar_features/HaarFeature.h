#pragma once
#include "inc/haar_features/EvaluateRactangle.h"
#include "inc/images/WindowIntegralImage.h"
#include "inc/support/PlatformHelperMacro.h"
#include "inc/haar_features/HaarFeatureCompact.h"

class HaarFeature
{
public:
	HaarFeature();
	HaarFeature(const EvaluateRactangle& r1, const EvaluateRactangle& r2);
	HaarFeature(const EvaluateRactangle& r1, const EvaluateRactangle& r2, const EvaluateRactangle& r3);

	inline const EvaluateRactangle& rect_1() const
	{
		return _r1;
	}
	inline const EvaluateRactangle& rect_2() const
	{
		return _r2;
	}
	inline const EvaluateRactangle& rect_3() const
	{
		return _r3;
	}
	inline bool is_three_rect() const
	{
		return _is_three_rect;
	}

	double FASTCALL(evaluate(const WindowIntegralImage &img) const);
	double FASTCALL(evaluate_two_rect(const WindowIntegralImage &img) const);
	double FASTCALL(evaluate_three_rect(const WindowIntegralImage &img) const);

	HaarFeature copy_scaled(double ratio);
	HaarFeatureTwoRectCompact create_compact_two_rect(double ratio);
	HaarFeatureThreeRectCompact create_compact_three_rect(double ratio);

private:
	EvaluateRactangle _r1, _r2, _r3;
	bool _is_three_rect;
};