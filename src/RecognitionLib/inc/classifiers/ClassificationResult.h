#pragma once
#include <assert.h>
#include "inc/classifiers/ClassificationTag.h"

class ClassificationResult
{
public:
	ClassificationResult()
	{
		_confidence = 0;
	}
	explicit ClassificationResult(ClassificationTag tag)
	{
		_confidence = 1;
		_tag = tag;
	}
	ClassificationResult(ClassificationTag tag, double confidence)
	{
		assert(confidence >= 0);
		_tag = tag;
		_confidence = confidence;
	}

	inline ClassificationTag tag() const
	{
		return _tag;
	}

	inline double confidence() const
	{
		return _confidence;
	}
	inline bool is_not_decided()
	{
		return _confidence == 0;
	}

	inline bool operator == (const ClassificationResult& other) const
	{
		return ((this->_tag == other._tag) && (this->_confidence == other._confidence));
	}
	inline bool operator != (const ClassificationResult& other) const
	{
		return ((this->_tag != other._tag) || (this->_confidence != other._confidence));
	}
	inline bool operator > (const ClassificationResult& other) const
	{
		if (this->_confidence == other._confidence)
			return this->_tag.value() > other._tag.value();
		return this->_confidence > other._confidence;
	}
	inline bool operator >= (const ClassificationResult& other) const
	{
		if (this->_confidence == other._confidence)
			return this->_tag.value() >= other._tag.value();
		return this->_confidence >= other._confidence;
	}
	inline bool operator < (const ClassificationResult& other) const
	{
		if (this->_confidence == other._confidence)
			return this->_tag.value() < other._tag.value();
		return this->_confidence < other._confidence;
	}
	inline bool operator <= (const ClassificationResult& other) const
	{
		if (this->_confidence == other._confidence)
			return this->_tag.value() <= other._tag.value();
		return this->_confidence <= other._confidence;
	}


private:
	ClassificationTag _tag;
	double _confidence;
};