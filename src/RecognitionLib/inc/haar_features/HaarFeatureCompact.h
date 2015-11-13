#pragma once
#include "inc/haar_features/EvaluateRactangleCompact.h"
#include "inc/images/WindowIntegralImage.h"
#include "inc/support/PlatformHelperMacro.h"

class HaarFeatureTwoRectCompact
{
public:
	HaarFeatureTwoRectCompact();
	HaarFeatureTwoRectCompact(const EvaluateRactangleCompact& r1, const EvaluateRactangleCompact& r2);

	float FASTCALL(evaluate(const WindowIntegralImage &img) const);
private:
	EvaluateRactangleCompact _r1, _r2;
};


class HaarFeatureThreeRectCompact
{
public:
	HaarFeatureThreeRectCompact();
	HaarFeatureThreeRectCompact(const EvaluateRactangleCompact& r1, const EvaluateRactangleCompact& r2, const EvaluateRactangleCompact& r3);

	float FASTCALL(evaluate(const WindowIntegralImage &img) const);
private:
	EvaluateRactangleCompact _r1, _r2, _r3;
};