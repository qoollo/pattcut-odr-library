#pragma once
#include <assert.h>
#include <stdint.h>
#include "inc/images/IntegralImage.h"
#include "inc/support/OwnTrackingPtr.h"
#include "inc/images/WindowData.h"

class WindowIntegralImage
{
public:
	WindowIntegralImage();
	WindowIntegralImage(const IntegralImage* img);
	WindowIntegralImage(std::shared_ptr<IntegralImage> img);

	void update_window_statistic();
	void set_window_size(int width, int height);
	void set_window_position(int x, int y);
	void set_window_params(int x, int y, int width, int height);

	WindowData get_window_data(int x, int y, int width, int height) const;
	void set_window_position(const WindowData& data);
	void set_window_params(const WindowData& data);

	inline WindowData get_window_data(int x, int y) const
	{
		return get_window_data(x, y, _wnd_width, _wnd_height);
	}
	inline WindowData get_window_data() const
	{
		return get_window_data(_wnd_x_pos, _wnd_y_pos, _wnd_width, _wnd_height);
	}


	inline int get_image_width() const
	{
		return _loc_int_image->get_width();
	}
	inline int get_image_height() const
	{
		return _loc_int_image->get_height();
	}
	inline int get_width() const
	{
		return _wnd_width;
	}
	inline int get_height() const
	{
		return _wnd_height;
	}
	inline int get_x_position() const
	{
		return _wnd_x_pos;
	}
	inline int get_y_position() const
	{
		return _wnd_y_pos;
	}
	inline float get_mean() const
	{
		return _mean;
	}
	inline float get_variance() const
	{
		return _variance;
	}
	inline float get_inv_variance() const
	{
		return _inv_variance;
	}
	inline int get_max_window_x_position() const
	{
		return _loc_int_image->get_width() - _wnd_width;
	}
	inline int get_max_window_y_position() const
	{
		return _loc_int_image->get_height() - _wnd_height;
	}
	

	inline uint32_t get_sum_in_rectangle(int x1, int y1, int x2, int y2) const
	{
		assert(x1 >= 0 && x1 <= _wnd_width);
		assert(y1 >= 0 && y1 <= _wnd_height);
		assert(x2 >= 0 && x2 <= _wnd_width + 1);
		assert(y2 >= 0 && y2 <= _wnd_height + 1);
		assert((x1 + _wnd_x_pos) >= 0 && (x1 + _wnd_x_pos) <= _loc_int_image->get_width());
		assert((y1 + _wnd_y_pos) >= 0 && (y1 + _wnd_y_pos) <= _loc_int_image->get_height());
		assert((x2 + _wnd_x_pos) >= 0 && (x2 + _wnd_x_pos) <= _loc_int_image->get_width());
		assert((y2 + _wnd_y_pos) >= 0 && (y2 + _wnd_y_pos) <= _loc_int_image->get_height());

		x1 += _wnd_x_pos;
		x2 += _wnd_x_pos;
		return _cached_row_pointer[y1][x1] - _cached_row_pointer[y1][x2] + _cached_row_pointer[y2][x2] - _cached_row_pointer[y2][x1];
	}
	inline uint32_t get_sum_in_rectangle_wh(int x, int y, int width, int height) const
	{
		return _loc_int_image->get_sum_in_rectangle_wh(x + _wnd_x_pos, y + _wnd_y_pos, x + _wnd_x_pos + width, y + _wnd_y_pos + height);
	}

	inline uint64_t get_square_sum_in_rectangle(int x1, int y1, int x2, int y2) const
	{
		return _loc_int_image->get_square_sum_in_rectangle(x1 + _wnd_x_pos, y1 + _wnd_y_pos, x2 + _wnd_x_pos, y2 + _wnd_y_pos);
	}
	inline uint64_t get_square_sum_in_rectangle_wh(int x, int y, int width, int height) const
	{
		return _loc_int_image->get_square_sum_in_rectangle_wh(x + _wnd_x_pos, y + _wnd_y_pos, x + _wnd_x_pos + width, y + _wnd_y_pos + height);
	}

	inline double get_normalized_sum_in_rectangle(int x1, int y1, int x2, int y2) const
	{
		return (_loc_int_image->get_sum_in_rectangle(x1 + _wnd_x_pos, y1 + _wnd_y_pos, x2 + _wnd_x_pos, y2 + _wnd_y_pos) * _inv_variance);
	}
	inline double get_normalized_sum_in_rectangle_wh(int x, int y, int width, int height) const
	{
		return (_loc_int_image->get_sum_in_rectangle_wh(x + _wnd_x_pos, y + _wnd_y_pos, x + _wnd_x_pos + width, y + _wnd_y_pos + height) * _inv_variance);
	}


private:
	OwnTrackingPtr<const IntegralImage> _loc_int_image;
	uint32_t const* const* _cached_row_pointer;

	int _wnd_x_pos;
	int _wnd_y_pos;
	int _wnd_width;
	int _wnd_height;

	int _area;
	float _inv_area;

	float _mean;
	float _variance;
	float _inv_variance;
};