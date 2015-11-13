#pragma once
#include <assert.h>
#include <stdint.h>

class WindowData
{
public:
	WindowData()
	{
		_x = 0;
		_y = 0;
		_width = 0;
		_height = 0;
		_sum = 0;
		_accum_variance = 0;
	}
	WindowData(int x, int y, int width, int height, uint32_t sum, uint64_t accum_variance)
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(width > 0);
		assert(height > 0);

		_x = x;
		_y = y;
		_width = width;
		_height = height;
		_sum = sum;
		_accum_variance = accum_variance;
	}

	inline int x() const
	{
		return _x;
	}
	inline int y() const
	{
		return _y;
	}
	inline int width() const
	{
		return _width;
	}
	inline int height() const
	{
		return _height;
	}

	inline uint32_t sum() const
	{
		return _sum;
	}
	inline uint64_t accum_variance() const
	{
		return _accum_variance;
	}

private:
	int _x;
	int _y;
	int _width;
	int _height;

	uint32_t _sum;
	uint64_t _accum_variance;
};