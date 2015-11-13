#pragma once
#include <assert.h>
#include <algorithm>

class Rectangle
{
public:
	Rectangle()
	{
		_x = 0;
		_y = 0;
		_width = 0;
		_height = 0;
	}

	Rectangle(int x, int y, int width, int height)
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
	
	inline int& left()
	{
		return _x;
	}
	inline int left() const
	{
		return _x;
	}

	inline int& top()
	{
		return _y;
	}
	inline int top() const
	{
		return _y;
	}

	inline int& width()
	{
		return _width;
	}
	inline int width() const
	{
		return _width;
	}

	inline int& height()
	{
		return _height;
	}
	inline int height() const
	{
		return _height;
	}

	inline int area() const
	{
		return _width * _height;
	}

	static Rectangle combine(const Rectangle &a, const Rectangle& b)
	{
		int new_x = std::min(a._x, b._x);
		int new_y = std::min(a._y, b._y);
		int new_right = std::max(a._x + a._width, b._x + b._width);
		int new_bottom = std::max(a._y + a._height, b._y + b._height);

		if (new_right < new_x)
			new_right = new_x;
		if (new_bottom < new_y)
			new_bottom = new_y;

		return Rectangle(new_x, new_y, new_right - new_x, new_bottom - new_y);
	}

	static Rectangle intersect(const Rectangle& a, const Rectangle& b)
	{
		int new_x = std::max(a._x, b._x);
		int new_y = std::max(a._y, b._y);
		int new_right = std::min(a._x + a._width, b._x + b._width);
		int new_bottom = std::min(a._y + a._height, b._y + b._height);

		if (new_right < new_x)
			new_right = new_x;
		if (new_bottom < new_y)
			new_bottom = new_y;

		return Rectangle(new_x, new_y, new_right - new_x, new_bottom - new_y);
	}

private:
	int _x;
	int _y;
	int _width;
	int _height;
};