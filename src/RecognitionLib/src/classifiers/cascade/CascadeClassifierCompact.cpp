#include "inc/classifiers/cascade/CascadeClassifierCompact.h"

CascadeClassifierCompact::CascadeClassifierCompact()
{

}

CascadeClassifierCompact::CascadeClassifierCompact(const std::vector<BoostedClassifierCompact>& inner_classifiers, ClassificationTag correct_tag, ClassificationTag incorrect_tag)
{
	_inner_classifiers = inner_classifiers;
	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;
}

CascadeClassifierCompact::CascadeClassifierCompact(std::vector<BoostedClassifierCompact>&& inner_classifiers, ClassificationTag correct_tag, ClassificationTag incorrect_tag)
{
	_inner_classifiers = std::move(inner_classifiers);
	_correct_tag = correct_tag;
	_incorrect_tag = incorrect_tag;

	if (_inner_classifiers.size() != _inner_classifiers.capacity())
		_inner_classifiers.shrink_to_fit();
}

CascadeClassifierCompact::CascadeClassifierCompact(const CascadeClassifierCompact& other)
{
	this->_inner_classifiers = other._inner_classifiers;
	this->_correct_tag = other._correct_tag;
	this->_incorrect_tag = other._incorrect_tag;
}

CascadeClassifierCompact::CascadeClassifierCompact(CascadeClassifierCompact&& other)
{
	_inner_classifiers = std::move(other._inner_classifiers);
	_correct_tag = other._correct_tag;
	_incorrect_tag = other._incorrect_tag;
}

CascadeClassifierCompact& CascadeClassifierCompact::operator=(const CascadeClassifierCompact& other)
{
	if (this != &other)
	{
		this->_inner_classifiers = other._inner_classifiers;
		this->_correct_tag = other._correct_tag;
		this->_incorrect_tag = other._incorrect_tag;
	}
	return *this;
}

CascadeClassifierCompact& CascadeClassifierCompact::operator=(CascadeClassifierCompact&& other)
{
	if (this != &other)
	{
		_inner_classifiers = std::move(other._inner_classifiers);
		_correct_tag = other._correct_tag;
		_incorrect_tag = other._incorrect_tag;
	}
	return *this;
}


ClassificationTag CascadeClassifierCompact::evaluate_tag(const WindowIntegralImage& image) const
{
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
	{
		if (_inner_classifiers[i].evaluate(image) < 0)
			return _incorrect_tag;
	}

	return _correct_tag;
}


CascadeStatistic CascadeClassifierCompact::evaluate_with_statistic(const WindowIntegralImage& image) const
{
	float last_classification_result;
	double combined_value = 0;

#ifdef __INTEL_COMPILER 
	#pragma forceinline 
#endif
	for (unsigned int i = 0; i < _inner_classifiers.size(); i++)
	{
		last_classification_result = _inner_classifiers[i].evaluate(image);

		if (last_classification_result < 0)
			return CascadeStatistic(ClassificationResult(_incorrect_tag, -last_classification_result), combined_value, i + 1);

		combined_value += last_classification_result * _inner_classifiers[i].get_classifier_count();
	}

	return CascadeStatistic(ClassificationResult(_correct_tag, last_classification_result), combined_value, (int)_inner_classifiers.size());
}
