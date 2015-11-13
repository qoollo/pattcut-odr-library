#pragma once
#include <vector>
#include <assert.h>
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/boosted/BoostedClassifierCompact.h"
#include "inc/classifiers/cascade/CascadeStatistic.h"


class CascadeClassifierCompact
{
public:
	CascadeClassifierCompact();
	CascadeClassifierCompact(const std::vector<BoostedClassifierCompact>& inner_classifiers, ClassificationTag correct_tag, ClassificationTag incorrect_tag);
	CascadeClassifierCompact(std::vector<BoostedClassifierCompact>&& inner_classifiers, ClassificationTag correct_tag, ClassificationTag incorrect_tag);
	CascadeClassifierCompact(const CascadeClassifierCompact& other);
	CascadeClassifierCompact(CascadeClassifierCompact&& other);

	CascadeClassifierCompact& operator=(const CascadeClassifierCompact& other);
	CascadeClassifierCompact& operator=(CascadeClassifierCompact&& other);

	inline size_t get_classifier_count() const
	{
		return _inner_classifiers.size();
	}

	ClassificationTag evaluate_tag(const WindowIntegralImage& image) const;
	CascadeStatistic evaluate_with_statistic(const WindowIntegralImage& image) const;

private:
	std::vector<BoostedClassifierCompact> _inner_classifiers;
	ClassificationTag _correct_tag;
	ClassificationTag _incorrect_tag;
};