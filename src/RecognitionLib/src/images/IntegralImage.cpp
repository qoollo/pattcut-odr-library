#include "inc/images/IntegralImage.h"
#include <math.h>
#include <utility>

IntegralImage::IntegralImage()
{
	_width = 0;
	_height = 0;
	_mean = 0;
	_variance = 0;
}

IntegralImage::IntegralImage(int width, int height)
{
	if (width * height > (1 << 24))
		throw LibraryException("Image is too large");

	_width = width;
	_height = height;

	_actual_data = OwnTrackingPtr<const Image>(std::make_shared<Image>(width, height));
	_integral_data = MatrixStorage<uint32_t>(height + 1, width + 1);
	_sq_integral_data = MatrixStorage<uint64_t>(height + 1, width + 1);

	_mean = 0;
	_variance = 0;
}

IntegralImage::IntegralImage(const IntegralImage& other)
{
	_width = other._width;
	_height = other._height;
	_mean = other._mean;
	_variance = other._variance;
	_actual_data = other._actual_data;
	_integral_data = other._integral_data;
	_sq_integral_data = other._sq_integral_data;
}

IntegralImage::IntegralImage(IntegralImage&& other)
{
	_width = other._width;
	_height = other._height;
	_mean = other._mean;
	_variance = other._variance;
	_actual_data = std::move(other._actual_data);
	_integral_data = std::move(other._integral_data);
	_sq_integral_data = std::move(other._sq_integral_data);
	
	other._width = 0;
	other._height = 0;
	other._mean = 0;
	other._variance = 0;
}

IntegralImage::IntegralImage(const Image* img)
{
	if (img->get_width() * img->get_height() > (1 << 24))
		throw LibraryException("Image is too large");

	_width = img->get_width();
	_height = img->get_height();

	_actual_data = OwnTrackingPtr<const Image>(img);
	_integral_data = MatrixStorage<uint32_t>(img->get_height() + 1, img->get_width() + 1);
	_sq_integral_data = MatrixStorage<uint64_t>(img->get_height() + 1, img->get_width() + 1);

	_mean = 0;
	_variance = 0;

	update_integral_data();
}

IntegralImage::IntegralImage(std::shared_ptr<Image> img)
{
	if (img->get_width() * img->get_height() > (1 << 24))
		throw LibraryException("Image is too large");

	_width = img->get_width();
	_height = img->get_height();

	_actual_data = OwnTrackingPtr<const Image>(img);
	_integral_data = MatrixStorage<uint32_t>(img->get_height() + 1, img->get_width() + 1);
	_sq_integral_data = MatrixStorage<uint64_t>(img->get_height() + 1, img->get_width() + 1);

	_mean = 0;
	_variance = 0;

	update_integral_data();
}


IntegralImage& IntegralImage::operator = (const IntegralImage& other)
{
	if (this != &other)
	{
		_width = other._width;
		_height = other._height;
		_mean = other._mean;
		_variance = other._variance;
		_actual_data = other._actual_data;
		_integral_data = other._integral_data;
		_sq_integral_data = other._sq_integral_data;
	}
	return *this;
}

IntegralImage& IntegralImage::operator = (IntegralImage&& other)
{
	if (this != &other)
	{
		_width = other._width;
		_height = other._height;
		_mean = other._mean;
		_variance = other._variance;
		_actual_data = std::move(other._actual_data);
		_integral_data = std::move(other._integral_data);
		_sq_integral_data = std::move(other._sq_integral_data);

		other._width = 0;
		other._height = 0;
		other._mean = 0;
		other._variance = 0;
	}
	return *this;
}

void IntegralImage::update_integral_data()
{
	uint32_t tmp_image_val = 0;
	uint32_t cur_integral_x = 0;
	uint64_t cur_sq_integral_x = 0;

	for (int x = 0; x < _width + 1; x++)
    {
		_integral_data.set_xy(x, 0, 0);
		_sq_integral_data.set_xy(x, 0, 0);
	}

	for (int y = 1; y < _height + 1; y++)
	{
		cur_integral_x = 0;
		cur_sq_integral_x = 0;

		_integral_data.set_xy(0, y, 0);
		_sq_integral_data.set_xy(0, y, 0);

		uint32_t* prev_int = _integral_data.data_row(y - 1);
		uint64_t* prev_sq = _sq_integral_data.data_row(y - 1);

		for (int x = 1; x < _width + 1; x++)
		{
			tmp_image_val = _actual_data->get(x - 1, y - 1);

			cur_integral_x += tmp_image_val;
			cur_sq_integral_x += tmp_image_val * tmp_image_val;

			_integral_data.set_xy(x, y, prev_int[x] + cur_integral_x);			
			_sq_integral_data.set_xy(x, y, prev_sq[x] + cur_sq_integral_x);
		}
	}

	double sum = _integral_data.get_xy(_width, _height);
	double sqSum = (double)_sq_integral_data.get_xy(_width, _height);
	double sqMean = sqSum / (_width * _height);

	_mean = sum / (_width * _height);

	double tmpVr = sqMean - _mean * _mean;
	if (tmpVr > 0)
		_variance = sqrt(tmpVr);
	else
		_variance = 1;
}
