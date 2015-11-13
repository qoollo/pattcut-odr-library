#pragma once
#include <vector>
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/classifiers/simple/StumpClassifier.h"
#include "inc/support/PlatformHelperMacro.h"
#include "inc/classifiers/boosted/BoostedClassifierCompact.h"

class BoostedClassifier
{
public:
	BoostedClassifier();
	BoostedClassifier(const std::vector<StumpClassifier> &weak_classif, double threshold, const ClassificationTag &correct_tag, const ClassificationTag &incorrect_tag);
	BoostedClassifier(const std::vector<StumpClassifier> &weak_classif, double threshold);
	BoostedClassifier(const BoostedClassifier& other);
	BoostedClassifier(BoostedClassifier &&other);

	BoostedClassifier& operator=(const BoostedClassifier& other);
	BoostedClassifier& operator=(BoostedClassifier &&other);

	inline double get_threshold() const
	{
		return _threshold;
	}

	inline void set_threshold(double threshold)
	{
		_threshold = threshold;
	}

	inline ClassificationTag get_correct_tag() const
	{
		return _correct_tag;
	}

	inline void set_correct_tag(const ClassificationTag &correct_tag)
	{
		_correct_tag = correct_tag;
	}

	inline ClassificationTag get_incorrect_tag() const
	{
		return _incorrect_tag;
	}

	void set_incorrect_tag(const ClassificationTag &incorrect_tag)
	{
		_incorrect_tag = incorrect_tag;
	}

	inline size_t get_classifier_count() const
	{
		return _weak_classif.size();
	}

	inline const StumpClassifier& get_classifier_at(unsigned long index) const
	{
		assert(index < _weak_classif.size());
		return _weak_classif.at(index);
	}

	ClassificationResult FASTCALL(evaluate(const WindowIntegralImage &img) const);
	ClassificationTag FASTCALL(evaluate_tag(const WindowIntegralImage &img) const);
	double FASTCALL(evaluate_signed_confidence(const WindowIntegralImage &img) const);

	BoostedClassifier copy_scaled(double ratio);
	BoostedClassifierCompact create_compact(double ratio);

private:
	std::vector<StumpClassifier> _weak_classif;
	// For perf optimization
	std::vector<StumpClassifier> _weak_classif_two_rect;
	std::vector<StumpClassifier> _weak_classif_three_rect;

	double _threshold;
	ClassificationTag _correct_tag;
	ClassificationTag _incorrect_tag;


};