#pragma once
#include <assert.h>
#include <stdint.h>
#include "inc/images/WindowData.h"


class FixedAreaStatMomentsClassifier
{
public:
	FixedAreaStatMomentsClassifier()
	{
		_area_width = 0;
		_area_height = 0;

		_sum_min = 0;
		_sum_max = 0;

		_accum_variance_min = 0;
		_accum_variance_max = 0;
	}
	FixedAreaStatMomentsClassifier(int area_width, int area_height, uint32_t sum_min, uint32_t sum_max, uint64_t accum_variance_min, uint64_t accum_variance_max)
	{
		assert(area_width > 0);
		assert(area_height > 0);

		_area_width = area_width;
		_area_height = area_height;

		_sum_min = sum_min;
		_sum_max = sum_max;

		_accum_variance_min = accum_variance_min;
		_accum_variance_max = accum_variance_max;
	}

	inline int get_area_width() const
	{
		return _area_width;
	}
	inline int get_area_height() const
	{
		return _area_height;
	}

	inline uint32_t get_sum_min() const
	{
		return _sum_min;
	}
	inline uint32_t get_sum_max() const
	{
		return _sum_max;
	}

	inline uint64_t get_accum_variance_min() const
	{
		return _accum_variance_min;
	}
	inline uint64_t get_accum_variance_max() const
	{
		return _accum_variance_max;
	}


	inline bool evaluate_bool(uint32_t sum, uint64_t accum_variance) const
	{
		return 
			accum_variance >= _accum_variance_min &&
			sum >= _sum_min &&
			sum <= _sum_max &&
			accum_variance <= _accum_variance_max;
	}
	inline bool evaluate_bool(const WindowData& data) const
	{
		assert(data.width() == _area_width);
		assert(data.height() == _area_height);

		return evaluate_bool(data.sum(), data.accum_variance());
	}

private:
	int _area_width;
	int _area_height;

	uint32_t _sum_min;
	uint32_t _sum_max;

	uint64_t _accum_variance_min;
	uint64_t _accum_variance_max;
};
