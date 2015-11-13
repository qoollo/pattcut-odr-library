#include "inc/classifiers/boosted/BoostedClassifier.h"

BoostedClassifier::BoostedClassifier()
{
	_threshold = 0.0;
	_correct_tag = ClassificationTag(1);
	_incorrect_tag = ClassificationTag(0);
}

BoostedClassifier::BoostedClassifier(const std::vector<StumpClassifier> &weak_classif, double threshold,
									const ClassificationTag &correct_tag, const ClassificationTag &incorrect_tag)
{
	_weak_classif = weak_classif;
	_threshold = threshold;
	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;

	for (auto& weak_cls : _weak_classif)
	{
		weak_cls.init_signed_confidence(_correct_tag);
		if (weak_cls.get_feature().is_three_rect())
			_weak_classif_three_rect.push_back(weak_cls);
		else
			_weak_classif_two_rect.push_back(weak_cls);
	}
}

BoostedClassifier::BoostedClassifier(const std::vector<StumpClassifier> &weak_classif, double threshold)
{
	_weak_classif = weak_classif;
	_threshold = threshold;
	_correct_tag = ClassificationTag(1);
	_incorrect_tag = ClassificationTag(0);

	for (auto& weak_cls : _weak_classif)
	{
		weak_cls.init_signed_confidence(_correct_tag);
		if (weak_cls.get_feature().is_three_rect())
			_weak_classif_three_rect.push_back(weak_cls);
		else
			_weak_classif_two_rect.push_back(weak_cls);
	}
}

BoostedClassifier::BoostedClassifier(const BoostedClassifier& other)
{
	this->_weak_classif = other._weak_classif;
	this->_weak_classif_two_rect = other._weak_classif_two_rect;
	this->_weak_classif_three_rect = other._weak_classif_three_rect;
	this->_threshold = other._threshold;
	this->_correct_tag = other._correct_tag;
	this->_incorrect_tag = other._incorrect_tag;
}

BoostedClassifier::BoostedClassifier(BoostedClassifier &&other)
{
	this->_weak_classif = std::move(other._weak_classif);
	this->_weak_classif_two_rect = std::move(other._weak_classif_two_rect);
	this->_weak_classif_three_rect = std::move(other._weak_classif_three_rect);
	this->_threshold = other._threshold;
	this->_correct_tag = other._correct_tag;
	this->_incorrect_tag = other._incorrect_tag;
}

BoostedClassifier& BoostedClassifier::operator=(const BoostedClassifier& other)
{
	if (this != &other)
	{
		this->_weak_classif = other._weak_classif;
		this->_weak_classif_two_rect = other._weak_classif_two_rect;
		this->_weak_classif_three_rect = other._weak_classif_three_rect;
		this->_threshold = other._threshold;
		this->_correct_tag = other._correct_tag;
		this->_incorrect_tag = other._incorrect_tag;
	}
	return *this;
}

BoostedClassifier &BoostedClassifier::operator=(BoostedClassifier &&other)
{
	if (this != &other)
	{
		this->_weak_classif = std::move(other._weak_classif);
		this->_weak_classif_two_rect = std::move(other._weak_classif_two_rect);
		this->_weak_classif_three_rect = std::move(other._weak_classif_three_rect);
		this->_threshold = other._threshold;
		this->_correct_tag = other._correct_tag;
		this->_incorrect_tag = other._incorrect_tag;
	}
	return *this;
}

ClassificationResult BoostedClassifier::evaluate(const WindowIntegralImage &img) const
{
	double res_confidence = 0.0;
	for (size_t i = 0; i < _weak_classif.size(); i++)
		res_confidence += _weak_classif[i].evaluate_signed_confidence(img);

	if (res_confidence > _threshold)
		return ClassificationResult(_correct_tag, res_confidence - _threshold);
	else
		return ClassificationResult(_incorrect_tag, _threshold - res_confidence);

}

ClassificationTag BoostedClassifier::evaluate_tag(const WindowIntegralImage &img) const
{
	double res_confidence = 0.0;
	for (size_t i = 0; i < _weak_classif.size(); ++i)
		res_confidence += _weak_classif[i].evaluate_signed_confidence(img);

	if (res_confidence > _threshold)
		return _correct_tag;
	else
		return _incorrect_tag;
}


double BoostedClassifier::evaluate_signed_confidence(const WindowIntegralImage &img) const
{
	double res_confidence = 0.0;

#ifdef __INTEL_COMPILER 
	#pragma forceinline 
#endif
	for (size_t i = 0; i < _weak_classif_two_rect.size(); i++)
		res_confidence += _weak_classif_two_rect[i].evaluate_signed_confidence_for_two_rect(img);

#ifdef __INTEL_COMPILER 
	#pragma forceinline 
#endif
	for (size_t i = 0; i < _weak_classif_three_rect.size(); i++)
		res_confidence += _weak_classif_three_rect[i].evaluate_signed_confidence_for_three_rect(img);

	return res_confidence - _threshold;
}


BoostedClassifier BoostedClassifier::copy_scaled(double ratio)
{
	std::vector<StumpClassifier> scaled_classif(_weak_classif.size());
	for (size_t i = 0; i < _weak_classif.size(); i++)
		scaled_classif[i] = _weak_classif.at(i).copy_scaled(ratio);

	return BoostedClassifier(scaled_classif, _threshold, _correct_tag, _incorrect_tag);
}

BoostedClassifierCompact BoostedClassifier::create_compact(double ratio)
{
	std::vector<StumpClassifierTwoRectCompact> weak_classif_two_rect(_weak_classif_two_rect.size());
	for (size_t i = 0; i < _weak_classif_two_rect.size(); i++)
		weak_classif_two_rect[i] = _weak_classif_two_rect.at(i).create_compact_two_rect(ratio);

	std::vector<StumpClassifierThreeRectCompact> weak_classif_three_rect(_weak_classif_three_rect.size());
	for (size_t i = 0; i < _weak_classif_three_rect.size(); i++)
		weak_classif_three_rect[i] = _weak_classif_three_rect.at(i).create_compact_three_rect(ratio);

	return BoostedClassifierCompact(std::move(weak_classif_two_rect), std::move(weak_classif_three_rect), _threshold);
}

