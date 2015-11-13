#pragma once
#include <assert.h>
#include "inc/support/Exceptions.h"

class EvaluateRactangleCompact
{
public:
	EvaluateRactangleCompact();
	EvaluateRactangleCompact(short x1, short y1, short x2, short y2, float corrected_weight);
	EvaluateRactangleCompact(int x1, int y1, int x2, int y2, double corrected_weight);

	inline short get_width() const
	{
		return _x2 - _x1;
	}
	inline short get_height() const
	{
		return _y2 - _y1;
	}
	inline float get_corrected_weight() const
	{
		return _corrected_weight;
	}
	inline short get_x1() const
	{
		return _x1;
	}
	inline short get_y1() const
	{
		return _y1;
	}
	inline short get_x2() const
	{
		return _x2;
	}
	inline short get_y2() const
	{
		return _y2;
	}

private:
	short _x1, _y1;
	short _x2, _y2;
	float _corrected_weight;
};