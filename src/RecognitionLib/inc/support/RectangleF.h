#pragma once
#include <assert.h>
#include <math.h>

class RectangleF
{
public:
	RectangleF()
	{
		_x = 0;
		_y = 0;
		_width = 0;
		_height = 0;
	}

	RectangleF(double x, double y, double width, double height)
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(width >= 0);
		assert(height >= 0);

		_x = x;
		_y = y;
		_width = width;
		_height = height;
	}

	static RectangleF from_rectangle(const Rectangle& other)
	{
		RectangleF res;
		res._x = other.left();
		res._y = other.top();
		res._width = other.width();
		res._height = other.height();

		return res;
	}

	inline double& left()
	{
		return _x;
	}
	inline double left() const
	{
		return _x;
	}

	inline double& top()
	{
		return _y;
	}
	inline double top() const
	{
		return _y;
	}

	inline double& width()
	{
		return _width;
	}
	inline double width() const
	{
		return _width;
	}

	inline double& height()
	{
		return _height;
	}
	inline double height() const
	{
		return _height;
	}

	inline double right() const
	{
		return _x + _width;
	}

	inline double bottom() const
	{
		return _y + _height;
	}


	inline bool is_empty() const
	{
		return _width < 1E-7 && _height < 1E-7;
	}

	inline double area() const
	{
		return _width * _height;
	}

	static RectangleF intersect(const RectangleF& a, const RectangleF& b)
	{
		double new_x = fmax(a._x, b._x);
		double new_y = fmax(a._y, b._y);
		double new_right = fmin(a._x + a._width, b._x + b._width);
		double new_bottom = fmin(a._y + a._height, b._y + b._height);

		if (new_right < new_x)
			new_right = new_x;
		if (new_bottom < new_y)
			new_bottom = new_y;

		return RectangleF(new_x, new_y, new_right - new_x, new_bottom - new_y);
	}

	static RectangleF combine(const RectangleF& a, const RectangleF& b)
	{
		double new_x = fmin(a._x, b._x);
		double new_y = fmin(a._y, b._y);
		double new_right = fmax(a._x + a._width, b._x + b._width);
		double new_bottom = fmax(a._y + a._height, b._y + b._height);

		if (new_right < new_x)
			new_right = new_x;
		if (new_bottom < new_y)
			new_bottom = new_y;

		return RectangleF(new_x, new_y, new_right - new_x, new_bottom - new_y);
	}

private:
	double _x;
	double _y;
	double _width;
	double _height;
};