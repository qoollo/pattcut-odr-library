#pragma once
#include <assert.h>
#include "inc/support/Exceptions.h"
#include "inc/haar_features/EvaluateRactangleCompact.h"

class EvaluateRactangle
{
public:
	EvaluateRactangle();
	EvaluateRactangle(int x1, int y1, int x2, int y2, int weight);
	EvaluateRactangle(int x1, int y1, int x2, int y2, int weight, double correction);

	inline int get_width() const
	{
		return _x2 - _x1;
	}
	inline int get_height() const
	{
		return _y2 - _y1;
	}
	inline int get_square() const
	{
		return (_x2 - _x1) * (_y2 - _y1);
	}
	inline double get_correction() const
	{
		return _correction;
	}
	inline int get_weight() const
	{
		return _weight;
	}
	inline double get_corrected_weight() const
	{
		return _corrected_weight;
	}
	inline int get_x1() const
	{
		return _x1;
	}
	inline int get_y1() const
	{
		return _y1;
	}
	inline int get_x2() const
	{
		return _x2;
	}
	inline int get_y2() const
	{
		return _y2;
	}

	EvaluateRactangle copy_scaled(double ratio);
	EvaluateRactangleCompact create_compact(double ratio);

private:
	int _x1, _y1;
	int _x2, _y2;
	int _weight;
	double _correction;
	double _corrected_weight;
};