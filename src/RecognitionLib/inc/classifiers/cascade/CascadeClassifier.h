#pragma once
#include <vector>
#include <assert.h>
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/boosted/BoostedClassifier.h"
#include "inc/classifiers/cascade/CascadeStatistic.h"
#include "inc/classifiers/cascade/CascadeClassifierCompact.h"


class CascadeClassifier
{
public:
	CascadeClassifier();
	CascadeClassifier(const std::vector<BoostedClassifier>& inner_classifiers, ClassificationTag correct_tag, ClassificationTag incorrect_tag);
	CascadeClassifier(const std::vector<BoostedClassifier>& inner_classifiers);
	CascadeClassifier(const CascadeClassifier& other);
	CascadeClassifier(CascadeClassifier&& other);

	CascadeClassifier& operator=(const CascadeClassifier& other);
	CascadeClassifier& operator=(CascadeClassifier&& other);

	inline ClassificationTag get_correct_tag() const
	{
		return _correct_tag;
	}
	inline void set_correct_tag(ClassificationTag correct_tag)
	{
		_correct_tag = correct_tag;
	}

	inline ClassificationTag get_incorrect_tag() const
	{
		return _incorrect_tag;
	}
	inline void set_incorrect_tag(ClassificationTag incorrect_tag)
	{
		_incorrect_tag = incorrect_tag;
	}

	inline size_t get_classifier_count() const
	{
		return _inner_classifiers.size();
	}

	inline const BoostedClassifier& get_classifier_at(int index) const
	{
		assert(index >= 0);
		assert((unsigned int)index < _inner_classifiers.size());
		return _inner_classifiers[index];
	}

	ClassificationResult evaluate(const WindowIntegralImage& image) const;
	ClassificationTag evaluate_tag(const WindowIntegralImage& image) const;

	CascadeStatistic evaluate_with_statistic(const WindowIntegralImage& image) const;

	CascadeClassifier copy_scaled(double ratio);
	CascadeClassifierCompact create_compact(double ratio);

private:
	std::vector<BoostedClassifier> _inner_classifiers;
	ClassificationTag _correct_tag;
	ClassificationTag _incorrect_tag;
};