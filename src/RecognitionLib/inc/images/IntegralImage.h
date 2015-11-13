#pragma once
#include <stdint.h>
#include <assert.h>
#include "inc/images/Image.h"
#include "inc/support/OwnTrackingPtr.h"
#include "inc/support/Exceptions.h"

class IntegralImage
{
public:
	IntegralImage();
	IntegralImage(const IntegralImage& other);
	IntegralImage(IntegralImage&& other);
	IntegralImage(std::shared_ptr<Image> img);
	IntegralImage(const Image* img);
	IntegralImage& operator = (const IntegralImage& other);
	IntegralImage& operator = (IntegralImage&& other);

	inline int get_width() const
	{
		return _width;
	}
	inline int get_height() const
	{
		return _height;
	}

	inline double get_mean() const
	{
		return _mean;
	}
	inline double get_variance() const
	{
		return _variance;
	}


	inline uint8_t const* const* get_raw_image_data() const
	{
		return _actual_data->data();
	}
	inline uint32_t const* const* get_raw_integral_data() const
	{
		return _integral_data.data();
	}
	inline uint64_t const* const* get_raw_sq_integral_data() const
	{
		return _sq_integral_data.data();
	}


	inline uint32_t get_sum_in_rectangle(int x1, int y1, int x2, int y2) const
	{
		assert(x2 >= x1);
		assert(y2 >= y1);
		return _integral_data.get_xy(x1, y1) - _integral_data.get_xy(x2, y1) + _integral_data.get_xy(x2, y2) - _integral_data.get_xy(x1, y2);
	}
	inline uint32_t get_sum_in_rectangle_wh(int x, int y, int width, int height) const
	{
		assert(width >= 0);
		assert(height >= 0);
		return _integral_data.get_xy(x, y) + _integral_data.get_xy(x + width, y + height) - _integral_data.get_xy(x, y + height) - _integral_data.get_xy(x + width, y);
	}
	inline uint64_t get_square_sum_in_rectangle(int x1, int y1, int x2, int y2) const
	{
		assert(x2 >= x1);
		assert(y2 >= y1);
		return _sq_integral_data.get_xy(x1, y1) + _sq_integral_data.get_xy(x2, y2) - _sq_integral_data.get_xy(x1, y2) - _sq_integral_data.get_xy(x2, y1);
	}
	inline uint64_t get_square_sum_in_rectangle_wh(int x, int y, int width, int height) const
	{
		assert(width >= 0);
		assert(height >= 0);
		return _sq_integral_data.get_xy(x, y) + _sq_integral_data.get_xy(x + width, y + height) - _sq_integral_data.get_xy(x, y + height) - _sq_integral_data.get_xy(x + width, y);
	}

private:
	int _width, _height;
	double _mean;
	double _variance;
	OwnTrackingPtr<const Image> _actual_data;
	MatrixStorage<uint32_t> _integral_data;
	MatrixStorage<uint64_t> _sq_integral_data;

	IntegralImage(int width, int height);
	void update_integral_data();
};