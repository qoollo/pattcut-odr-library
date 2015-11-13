#pragma once
#include <stdint.h>
#include <assert.h>
#include <string.h>

template <typename T>
class MatrixStorage
{
public:	
	static MatrixStorage create_rc(const int row_count, const int column_count)
	{
		return MatrixStorage(row_count, column_count);
	}

	static MatrixStorage create_xy(const int width, const int height)
	{
		return MatrixStorage(height, width);
	}

	MatrixStorage()
	{
		_row_count = 0;
		_column_count = 0;
		_matrix_length = 0;
		_data = nullptr;
		_matrix = nullptr;
	}
	MatrixStorage(const int row_count, const int column_count)
	{
		assert(row_count > 0);
		assert(column_count > 0);

		_row_count = row_count;
		_column_count = column_count;
		_matrix_length = calculate_matrix_length(row_count, column_count);
		_matrix = new uint8_t[_matrix_length];		
		_data = construct_data_from_matrix(_matrix, _matrix_length, row_count, column_count);
	}
	MatrixStorage(const MatrixStorage& ms)
	{
		if (ms._matrix != nullptr)
		{
			_row_count = ms._row_count;
			_column_count = ms._column_count;
			_matrix_length = ms._matrix_length;
			_matrix = copy_memory(ms._matrix, _matrix_length);
			_data = construct_data_from_matrix(_matrix, _matrix_length, _row_count, _column_count);
		}
		else
		{
			_row_count = 0;
			_column_count = 0;
			_matrix_length = 0;
			_matrix = nullptr;
			_data = nullptr;
		}
	}
	MatrixStorage(MatrixStorage&& ms)
	{
		_row_count = ms._row_count;
		_column_count = ms._column_count;
		_matrix_length = ms._matrix_length;
		_matrix = ms._matrix;
		_data = ms._data;

		ms._row_count = 0;
		ms._column_count = 0;
		ms._matrix_length = 0;
		ms._matrix = nullptr;
		ms._data = nullptr;
	}
	
	~MatrixStorage()
	{
		if (_matrix != nullptr)
			delete[] _matrix;

		_row_count = 0;
		_column_count = 0;
		_matrix_length = 0;
		_matrix = nullptr;
		_data = nullptr;

	}


	MatrixStorage& operator= (const MatrixStorage& ms)
	{
		if (this == &ms)
			return *this;

		if (_matrix != nullptr)
		{
			delete[] _matrix;
			_matrix = nullptr;
			_data = nullptr;
		}

		if (ms._matrix != nullptr)
		{
			_row_count = ms._row_count;
			_column_count = ms._column_count;
			_matrix_length = ms._matrix_length;
			_matrix = copy_memory(ms._matrix, _matrix_length);
			_data = construct_data_from_matrix(_matrix, _matrix_length, _row_count, _column_count);
		}
		else
		{
			_row_count = 0;
			_column_count = 0;
			_matrix_length = 0;
			_matrix = nullptr;
			_data = nullptr;
		}
		return *this;
	}
	MatrixStorage& operator= (MatrixStorage&& ms)
	{
		if (this == &ms)
			return *this;

		if (_matrix != nullptr)
		{
			delete[] _matrix;
			_matrix = nullptr;
			_data = nullptr;
		}

		_row_count = ms._row_count;
		_column_count = ms._column_count;
		_matrix_length = ms._matrix_length;
		_matrix = ms._matrix;
		_data = ms._data;

		ms._row_count = 0;
		ms._column_count = 0;
		ms._matrix_length = 0;
		ms._matrix = nullptr;
		ms._data = nullptr;

		return *this;
	}

	inline T get_rc(const int row, const int column) const
	{
		assert(row >= 0);
		assert(row < _row_count);
		assert(column >= 0);
		assert(column < _column_count);
		assert(data != nullptr);
		return _data[row][column];
	}

	inline void set_rc(const int row, const int column, const T element)
	{
		assert(row >= 0);
		assert(row < _row_count);
		assert(column >= 0);
		assert(column < _column_count);
		assert(data != nullptr);
		_data[row][column] = element;
	}

	inline int get_row_count() const
	{
		return _row_count;
	}

	inline int get_column_count() const
	{
		return _column_count;
	}
	
	inline T get_xy(const int x, const int y) const
	{
		assert(x >= 0);
		assert(x < _column_count);
		assert(y >= 0);
		assert(y < _row_count);
		assert(_data != nullptr);
		return _data[y][x];
	}

	inline void set_xy(const int x, const int y, const T element)
	{
		assert(x >= 0);
		assert(x < _column_count);
		assert(y >= 0);
		assert(y < _row_count);
		assert(_data != nullptr);
		_data[y][x] = element;
	}

	inline int get_width() const
	{
		return _column_count;
	}

	inline int get_height() const
	{
		return _row_count;
	}

	T const* const* data() const
	{
		return _data;
	}
	T const* data_row(const int row) const
	{
		assert(row >= 0);
		assert(row < _row_count);
		assert(_data != nullptr);
		return _data[row];
	}
	T** data()
	{
		return _data;
	}
	T* data_row(const int row)
	{
		assert(row >= 0);
		assert(row < _row_count);
		assert(_data != nullptr);
		return _data[row];
	}

private:
	int _row_count;
	int _column_count;
	int _matrix_length;
	uint8_t* _matrix;
	T** _data;

	static const unsigned int cache_line_size = 64;

private:

	static unsigned int get_align_size(unsigned int size)
	{
		if ((size % cache_line_size) == 0)
			return size;
		return ((cache_line_size - (size % cache_line_size)) + size);
	}

	static unsigned int calculate_matrix_length(const int row_count, const int column_count)
	{
		assert(row_count > 0);
		assert(column_count > 0);

		int alignColumns = get_align_size(column_count * sizeof(T));
		int alignPointers = get_align_size(row_count * sizeof(T*));

		return row_count * alignColumns + alignPointers;
	}

	static uint8_t* copy_memory(const uint8_t* source, const size_t size)
	{
		uint8_t* result = new uint8_t[size];
		memcpy(result, source, size);
		return result;
	}

	static T** construct_data_from_matrix(uint8_t* matrix, const int matrix_length, const int row_count, const int column_count)
	{
		if (matrix == nullptr)
			return nullptr;

		int alignColumns = get_align_size(column_count * sizeof(T));
		int alignPointers = get_align_size(row_count * sizeof(T*));

		assert(matrix_length == row_count * alignColumns + alignPointers);

		T** data = (T**)matrix;
		for (int r = 0; r < row_count; r++)
			data[r] = (T*)(matrix + r * alignColumns + alignPointers);

		return data;
	}
};