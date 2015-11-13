#pragma once
#include <utility>
#include <stdint.h>
#include "inc/images/MatrixStorage.h"

class Image
{
public:
	Image() { }
	Image(const int width, const int height) : _matrix(height, width) { }
	Image(const Image& other) : _matrix(other._matrix) { }
	Image(Image&& other) : _matrix(std::move(other._matrix)) { }

	Image& operator=(const Image& other)
	{
		if (this == &other)
			return *this;
		this->_matrix = other._matrix;
		return *this;
	}

	Image& operator=(Image&& other)
	{
		if (this == &other)
			return *this;
		this->_matrix = std::move(other._matrix);
		return *this;
	}

	inline int get_width() const
	{
		return _matrix.get_width();
	}
	inline int get_height() const
	{
		return _matrix.get_height();
	}

	inline uint8_t get(const int x, const int y) const
	{
		return _matrix.get_xy(x, y);
	}
	inline void set(const int x, const int y, const uint8_t element)
	{
		_matrix.set_xy(x, y, element);
	}

	uint8_t const* const* data() const
	{
		return _matrix.data();
	}
	uint8_t ** data()
	{
		return _matrix.data();
	}
	uint8_t const* data_row(const int y) const
	{
		return _matrix.data_row(y);
	}
	uint8_t * data_row(const int y)
	{
		return _matrix.data_row(y);
	}

private:
	MatrixStorage<uint8_t> _matrix;
};