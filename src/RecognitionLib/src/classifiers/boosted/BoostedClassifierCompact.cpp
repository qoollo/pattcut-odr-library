#include "inc/classifiers/boosted/BoostedClassifierCompact.h"
#include <limits>

static float double_to_float(double src)
{
	if (src > (double)std::numeric_limits<float>().max())
		return std::numeric_limits<float>().max();
	if (src < (double)std::numeric_limits<float>().lowest())
		return std::numeric_limits<float>().lowest();

	return (float)src;
}

BoostedClassifierCompact::BoostedClassifierCompact()
{
	_threshold = 0.0f;
	_weak_classif_count = 0;
}

BoostedClassifierCompact::BoostedClassifierCompact(const std::vector<StumpClassifierTwoRectCompact>& weak_classif_two_rect, const std::vector<StumpClassifierThreeRectCompact>& weak_classif_three_rect, double threshold)
{
	_weak_classif_two_rect = weak_classif_two_rect;
	_weak_classif_three_rect = weak_classif_three_rect;
	_threshold = double_to_float(threshold);
	_weak_classif_count = (int)_weak_classif_two_rect.size() + (int)_weak_classif_three_rect.size();
}

BoostedClassifierCompact::BoostedClassifierCompact(std::vector<StumpClassifierTwoRectCompact>&& weak_classif_two_rect, std::vector<StumpClassifierThreeRectCompact>&& weak_classif_three_rect, double threshold)
{
	_weak_classif_two_rect = std::move(weak_classif_two_rect);
	_weak_classif_three_rect = std::move(weak_classif_three_rect);
	_threshold = double_to_float(threshold);
	_weak_classif_count = (int)_weak_classif_two_rect.size() + (int)_weak_classif_three_rect.size();

	if (_weak_classif_two_rect.size() != _weak_classif_two_rect.capacity())
		_weak_classif_two_rect.shrink_to_fit();
	if (_weak_classif_three_rect.size() != _weak_classif_three_rect.capacity())
		_weak_classif_three_rect.shrink_to_fit();
}

BoostedClassifierCompact::BoostedClassifierCompact(const BoostedClassifierCompact& other)
{
	this->_weak_classif_two_rect = other._weak_classif_two_rect;
	this->_weak_classif_three_rect = other._weak_classif_three_rect;
	this->_threshold = other._threshold;
	this->_weak_classif_count = other._weak_classif_count;
}

BoostedClassifierCompact::BoostedClassifierCompact(BoostedClassifierCompact &&other)
{
	this->_weak_classif_two_rect = std::move(other._weak_classif_two_rect);
	this->_weak_classif_three_rect = std::move(other._weak_classif_three_rect);
	this->_threshold = other._threshold;
	this->_weak_classif_count = other._weak_classif_count;
}

BoostedClassifierCompact& BoostedClassifierCompact::operator=(const BoostedClassifierCompact& other)
{
	if (this != &other)
	{
		this->_weak_classif_two_rect = other._weak_classif_two_rect;
		this->_weak_classif_three_rect = other._weak_classif_three_rect;
		this->_threshold = other._threshold;
		this->_weak_classif_count = other._weak_classif_count;
	}
	return *this;
}

BoostedClassifierCompact &BoostedClassifierCompact::operator=(BoostedClassifierCompact &&other)
{
	if (this != &other)
	{
		this->_weak_classif_two_rect = std::move(other._weak_classif_two_rect);
		this->_weak_classif_three_rect = std::move(other._weak_classif_three_rect);
		this->_threshold = other._threshold;
		this->_weak_classif_count = other._weak_classif_count;
	}
	return *this;
}


float BoostedClassifierCompact::evaluate(const WindowIntegralImage &img) const
{
	float res_confidence = 0.0;

#ifdef __INTEL_COMPILER 
	#pragma forceinline 
#endif
	for (size_t i = 0; i < _weak_classif_two_rect.size(); i++)
		res_confidence += _weak_classif_two_rect[i].evaluate(img);

#ifdef __INTEL_COMPILER 
	#pragma forceinline 
#endif
	for (size_t i = 0; i < _weak_classif_three_rect.size(); i++)
		res_confidence += _weak_classif_three_rect[i].evaluate(img);

	return res_confidence - _threshold;
}

