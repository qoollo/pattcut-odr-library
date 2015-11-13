#include "inc/images/WindowIntegralImage.h"
#include <math.h> 

WindowIntegralImage::WindowIntegralImage()
{
	_wnd_x_pos = 0;
	_wnd_y_pos = 0;
	_wnd_width = 0;
	_wnd_height = 0;

	_area = 0;
	_inv_area = 0;

	_mean = 0;
	_variance = 0;
	_inv_variance = 0;

	_cached_row_pointer = nullptr;
}

WindowIntegralImage::WindowIntegralImage(std::shared_ptr<IntegralImage> img) : _loc_int_image(img)
{
	_wnd_x_pos = 0;
	_wnd_y_pos = 0;
	_wnd_width = img->get_width();
	_wnd_height = img->get_height();

	_area = _wnd_width * _wnd_height;
	_inv_area = (float)(1.0 / _area);

	_mean = 0;
	_variance = 0;
	_inv_variance = 0;

	_cached_row_pointer = _loc_int_image->get_raw_integral_data() + _wnd_y_pos;

	update_window_statistic();
}

WindowIntegralImage::WindowIntegralImage(const IntegralImage* img) : _loc_int_image(img)
{
	_wnd_x_pos = 0;
	_wnd_y_pos = 0;
	_wnd_width = img->get_width();
	_wnd_height = img->get_height();

	_area = _wnd_width * _wnd_height;
	_inv_area = (float)(1.0 / _area);

	_mean = 0;
	_variance = 0;
	_inv_variance = 0;

	_cached_row_pointer = _loc_int_image->get_raw_integral_data() + _wnd_y_pos;

	update_window_statistic();
}


void WindowIntegralImage::set_window_size(int width, int height)
{
	assert(width > 0);
	assert(height > 0);
	assert(width + _wnd_x_pos <= _loc_int_image->get_width());
	assert(height + _wnd_y_pos <= _loc_int_image->get_height());

	_wnd_width = width;
	_wnd_height = height;

	_area = _wnd_width * _wnd_height;
	_inv_area = (float)(1.0 / _area);

	update_window_statistic();
}
void WindowIntegralImage::set_window_position(int x, int y)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x + _wnd_width <= _loc_int_image->get_width());
	assert(y + _wnd_height <= _loc_int_image->get_height());

	_wnd_x_pos = x;
	_wnd_y_pos = y;

	_cached_row_pointer = _loc_int_image->get_raw_integral_data() + _wnd_y_pos;

	update_window_statistic();
}
void WindowIntegralImage::set_window_params(int x, int y, int width, int height)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(width > 0);
	assert(height > 0);
	assert(x + width <= _loc_int_image->get_width());
	assert(y + height <= _loc_int_image->get_height());

	_wnd_x_pos = x;
	_wnd_y_pos = y;
	_wnd_width = width;
	_wnd_height = height;

	_area = _wnd_width * _wnd_height;
	_inv_area = (float)(1.0 / _area);

	_cached_row_pointer = _loc_int_image->get_raw_integral_data() + _wnd_y_pos;

	update_window_statistic();
}

void WindowIntegralImage::update_window_statistic()
{
	auto sum = get_sum_in_rectangle(0, 0, _wnd_width, _wnd_height);
	auto sq_sum = get_square_sum_in_rectangle(0, 0, _wnd_width, _wnd_height);

	uint64_t m_variance = (uint64_t)_area * sq_sum - (uint64_t)sum * sum;

	_mean = sum * _inv_area;
	_variance = sqrt((float)m_variance) * _inv_area;
	_inv_variance = 1.0f / _variance;
}

WindowData WindowIntegralImage::get_window_data(int x, int y, int width, int height) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(width > 0);
	assert(height > 0);
	assert(x + width <= _loc_int_image->get_width());
	assert(y + height <= _loc_int_image->get_height());

	auto sum = _loc_int_image->get_sum_in_rectangle(x, y, x + width, y + height);
	auto sq_sum = _loc_int_image->get_square_sum_in_rectangle(x, y, x + width, y + height);

	uint64_t m_variance = (uint64_t)_area * sq_sum - (uint64_t)sum * sum;

	return WindowData(x, y, width, height, sum, m_variance);
}

void WindowIntegralImage::set_window_position(const WindowData& data)
{
	assert(data.x() >= 0);
	assert(data.y() >= 0);
	assert(data.width() > 0);
	assert(data.height() > 0);
	assert(data.width() == _wnd_width);
	assert(data.height() == _wnd_height);
	assert(data.x() + data.width() <= _loc_int_image->get_width());
	assert(data.y() + data.height() <= _loc_int_image->get_height());

	_wnd_x_pos = data.x();
	_wnd_y_pos = data.y();

	_cached_row_pointer = _loc_int_image->get_raw_integral_data() + _wnd_y_pos;

	_mean = data.sum() * _inv_area;
	_variance = sqrt((float)data.accum_variance()) * _inv_area;
	_inv_variance = 1.0f / _variance;
}

void WindowIntegralImage::set_window_params(const WindowData& data)
{
	assert(data.x() >= 0);
	assert(data.y() >= 0);
	assert(data.width() > 0);
	assert(data.height() > 0);
	assert(data.x() + data.width() <= _loc_int_image->get_width());
	assert(data.y() + data.height() <= _loc_int_image->get_height());

	_wnd_x_pos = data.x();
	_wnd_y_pos = data.y();
	_wnd_width = data.width();
	_wnd_height = data.height();

	_area = _wnd_width * _wnd_height;
#if defined (_MSC_VER)
	if (_area != 0)
#endif
	_inv_area = (float)(1.0 / _area);

	_cached_row_pointer = _loc_int_image->get_raw_integral_data() + _wnd_y_pos;

	_mean = data.sum() * _inv_area;
	_variance = sqrt((float)data.accum_variance()) * _inv_area;
	_inv_variance = 1.0f / _variance;
}