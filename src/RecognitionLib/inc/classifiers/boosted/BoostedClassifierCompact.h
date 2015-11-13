#pragma once
#include <vector>
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/classifiers/simple/StumpClassifierCompact.h"
#include "inc/support/PlatformHelperMacro.h"

class BoostedClassifierCompact
{
public:
	BoostedClassifierCompact();
	BoostedClassifierCompact(const std::vector<StumpClassifierTwoRectCompact>& weak_classif_two_rect, const std::vector<StumpClassifierThreeRectCompact>& weak_classif_three_rect, double threshold);
	BoostedClassifierCompact(std::vector<StumpClassifierTwoRectCompact>&& weak_classif_two_rect, std::vector<StumpClassifierThreeRectCompact>&& weak_classif_three_rect, double threshold);
	BoostedClassifierCompact(const BoostedClassifierCompact& other);
	BoostedClassifierCompact(BoostedClassifierCompact &&other);

	BoostedClassifierCompact& operator=(const BoostedClassifierCompact& other);
	BoostedClassifierCompact& operator=(BoostedClassifierCompact &&other);

	inline int get_classifier_count() const
	{
		return _weak_classif_count;
	}

	float FASTCALL(evaluate(const WindowIntegralImage &img) const);

private:
	std::vector<StumpClassifierTwoRectCompact> _weak_classif_two_rect;
	std::vector<StumpClassifierThreeRectCompact> _weak_classif_three_rect;

	float _threshold;
	int _weak_classif_count;
};