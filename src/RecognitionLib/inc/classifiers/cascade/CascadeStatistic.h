#pragma once
#include <assert.h>
#include "inc/classifiers/ClassificationResult.h"

class CascadeStatistic
{
public:
	CascadeStatistic()
	{
		_combined_classification_value = 0;
		_depth = 0;
	}

	CascadeStatistic(const ClassificationResult& classification_result, double combined_classification_value, int depth)
	{
		assert(depth >= 0);
		_classification_result = classification_result;
		_combined_classification_value = combined_classification_value;
		_depth = depth;
	}

	inline ClassificationResult get_classification_result() const
	{
		return _classification_result;
	}

	inline double get_combined_clasification_value() const
	{
		return _combined_classification_value;
	}

	inline int get_depth() const
	{
		return _depth;
	}

private:
	ClassificationResult _classification_result;
	double _combined_classification_value;
	int _depth;
};