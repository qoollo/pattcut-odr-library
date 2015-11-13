#pragma once
#include <assert.h>
#include "inc/support/Rectangle.h"

class FilteredAreaInfo
{
public:
	FilteredAreaInfo()
	{
		_wnd_count = 0;
	}

	FilteredAreaInfo(const Rectangle& area, int wnd_count)
	{
		assert(wnd_count > 0);
		_area = area;
		_wnd_count = wnd_count;
	}

	inline const Rectangle& get_area() const
	{
		return _area;
	}

	inline int get_wnd_count() const
	{
		return _wnd_count;
	}

private:
	Rectangle _area;
	int _wnd_count;
};