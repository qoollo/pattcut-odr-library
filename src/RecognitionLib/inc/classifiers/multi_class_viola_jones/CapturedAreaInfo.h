#pragma once
#include "inc/accuracy_estimation/AccuracyResults.h"
#include "inc/post_filtration/FilteredAreaWithExtInfo.h"

class CapturedAreaInfo
{
public:
	CapturedAreaInfo()
	{
		_class_id = 0;
		_accuracy = 0;
		_is_good = false;
	}

	CapturedAreaInfo(int class_id, const FilteredAreaWithExtInfo& inner_area, double accuracy, bool is_good)
	{
		_class_id = class_id;
		_inner_area_info = inner_area;
		_accuracy = accuracy;
		_is_good = is_good;
	}

	CapturedAreaInfo(int class_id, const FilteredAreaWithExtInfo& inner_area, const AccuracyResults& acc)
	{
		_class_id = class_id;
		_inner_area_info = inner_area;
		_accuracy = acc.accuracy();
		_is_good = acc.is_good();
	}

	inline int get_class_id() const
	{
		return _class_id;
	}

	inline const FilteredAreaWithExtInfo& get_inner_area_info() const
	{
		return _inner_area_info;
	}

	inline double get_accuracy() const
	{
		return _accuracy;
	}

	inline bool get_is_good() const
	{
		return _is_good;
	}

private:
	int _class_id;
	FilteredAreaWithExtInfo _inner_area_info;
	double _accuracy;
	bool _is_good;
};