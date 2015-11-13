#pragma once
#include "inc/support/Rectangle.h"
#include "inc/classifiers/ClassificationResult.h"

class AreaWithExtInfo
{
public:
	AreaWithExtInfo()
	{
		_comd_boosted_classifier_value = 0;
	}

	AreaWithExtInfo(Rectangle area, const ClassificationResult& last_boosted_classifier_result, double comd_boosted_classifier_value)
	{
		_area = area;
		_last_boosted_classifier_result = last_boosted_classifier_result;
		_comd_boosted_classifier_value = comd_boosted_classifier_value;
	}

	inline const Rectangle& get_area() const
	{
		return _area;
	}

	inline double get_last_boosted_classifier_confidence() const
	{
		return _last_boosted_classifier_result.confidence();
	}

	inline const ClassificationResult& get_last_boosted_classifier_result() const
	{
		return _last_boosted_classifier_result;
	}

	inline double get_comd_boosted_classifier_value() const
	{
		return _comd_boosted_classifier_value;
	}

private:
	Rectangle _area;
	ClassificationResult _last_boosted_classifier_result;
	double _comd_boosted_classifier_value;
};