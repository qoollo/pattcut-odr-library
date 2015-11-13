#include "inc/classifiers/cascade/CascadeClassifier.h"

CascadeClassifier::CascadeClassifier()
{

}

CascadeClassifier::CascadeClassifier(const std::vector<BoostedClassifier>& inner_classifiers, ClassificationTag correct_tag, ClassificationTag incorrect_tag)
{
	_inner_classifiers = inner_classifiers;
	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;
}

CascadeClassifier::CascadeClassifier(const std::vector<BoostedClassifier>& inner_classifiers)
{
	_inner_classifiers = inner_classifiers;
	_correct_tag = ClassificationTag(1);
	_incorrect_tag = ClassificationTag(0);
}

CascadeClassifier::CascadeClassifier(const CascadeClassifier& other)
{
	this->_inner_classifiers = other._inner_classifiers;
	this->_correct_tag = other._correct_tag;
	this->_incorrect_tag = other._incorrect_tag;
}

CascadeClassifier::CascadeClassifier(CascadeClassifier&& other)
{
	_inner_classifiers = std::move(other._inner_classifiers);
	_correct_tag = other._correct_tag;
	_incorrect_tag = other._incorrect_tag;
}

CascadeClassifier& CascadeClassifier::operator=(const CascadeClassifier& other)
{
	if (this != &other)
	{
		this->_inner_classifiers = other._inner_classifiers;
		this->_correct_tag = other._correct_tag;
		this->_incorrect_tag = other._incorrect_tag;
	}
	return *this;
}

CascadeClassifier& CascadeClassifier::operator=(CascadeClassifier&& other)
{
	if (this != &other)
	{
		_inner_classifiers = std::move(other._inner_classifiers);
		_correct_tag = other._correct_tag;
		_incorrect_tag = other._incorrect_tag;
	}
	return *this;
}

ClassificationResult CascadeClassifier::evaluate(const WindowIntegralImage& image) const
{
	double last_eval_confidense = 0;
	
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
	{
		auto eval_result = _inner_classifiers[i].evaluate(image);
		last_eval_confidense = eval_result.confidence();

		if (eval_result.tag() != _correct_tag)
			return ClassificationResult(_correct_tag, eval_result.confidence());
	}

	return ClassificationResult(_correct_tag, last_eval_confidense);
}

ClassificationTag CascadeClassifier::evaluate_tag(const WindowIntegralImage& image) const
{
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
	{
		auto eval_result = _inner_classifiers[i].evaluate(image);

		if (eval_result.tag() != _correct_tag)
			return _incorrect_tag;
	}

	return _correct_tag;
}

CascadeStatistic CascadeClassifier::evaluate_with_statistic(const WindowIntegralImage& image) const
{
	double last_classification_result;
	double combined_value = 0;

#ifdef __INTEL_COMPILER 
	#pragma forceinline 
#endif
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
	{
		last_classification_result = _inner_classifiers[i].evaluate_signed_confidence(image);

		if (last_classification_result < 0)
			return CascadeStatistic(ClassificationResult(_incorrect_tag, -last_classification_result), combined_value, i + 1);

		combined_value += last_classification_result * _inner_classifiers[i].get_classifier_count();
	}

	return CascadeStatistic(ClassificationResult(_correct_tag, last_classification_result), combined_value, (int)_inner_classifiers.size());
}

CascadeClassifier CascadeClassifier::copy_scaled(double ratio)
{
	std::vector<BoostedClassifier> inner(_inner_classifiers.size());
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
		inner[i] = _inner_classifiers[i].copy_scaled(ratio);

	return CascadeClassifier(inner, _correct_tag, _incorrect_tag);
}

CascadeClassifierCompact CascadeClassifier::create_compact(double ratio)
{
	std::vector<BoostedClassifierCompact> inner(_inner_classifiers.size());
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
		inner[i] = _inner_classifiers[i].create_compact(ratio);

	return CascadeClassifierCompact(std::move(inner), _correct_tag, _incorrect_tag);
}
