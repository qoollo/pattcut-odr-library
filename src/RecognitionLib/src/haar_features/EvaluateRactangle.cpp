#include "inc/haar_features/EvaluateRactangle.h"
#include <math.h>

EvaluateRactangle::EvaluateRactangle()
{
	_x1 = 0;
	_y1 = 0;
	_x2 = 0;
	_y2 = 0;

	_weight = 0;
	_correction = 0;
	_corrected_weight = 0;
}

EvaluateRactangle::EvaluateRactangle(int x1, int y1, int x2, int y2, int weight)
{
	assert(x1 >= 0);
	assert(y1 >= 0);
	assert(x2 >= 0);
	assert(y2 >= 0);
	assert(x2 >= x1);
	assert(y2 >= y1);
	assert(weight != 0);

	_x1 = x1;
	_y1 = y1;
	_x2 = x2;
	_y2 = y2;

	_weight = weight;
	_correction = 1;
	_corrected_weight = _weight * _correction;
}

EvaluateRactangle::EvaluateRactangle(int x1, int y1, int x2, int y2, int weight, double correction)
{
	assert(x1 >= 0);
	assert(y1 >= 0);
	assert(x2 >= 0);
	assert(y2 >= 0);
	assert(x2 >= x1);
	assert(y2 >= y1);
	assert(weight != 0);
	assert(correction > 0);

	_x1 = x1;
	_y1 = y1;
	_x2 = x2;
	_y2 = y2;

	_weight = weight;
	_correction = correction;
	_corrected_weight = _weight * _correction;
}

EvaluateRactangle EvaluateRactangle::copy_scaled(double ratio)
{
	assert(ratio > 0);
	assert(_correction == 1);

	int n_x1 = (int)(round(ratio * _x1));
	int n_y1 = (int)(round(ratio * _y1));
	int n_x2 = (int)(round(ratio * get_width())) + n_x1;
	int n_y2 = (int)(round(ratio * get_height())) + n_y1;

	if (n_x1 == n_x2 || n_y1 == n_y2)
		throw InvalidScaleException("EvaluateRectange has a zero width or height");

	double correction = fabs((get_width() * get_height() * ratio * ratio) / ((n_x2 - n_x1) * (n_y2 - n_y1)));

	return EvaluateRactangle(n_x1, n_y1, n_x2, n_y2, _weight, correction * _correction);
}


EvaluateRactangleCompact EvaluateRactangle::create_compact(double ratio)
{
	assert(ratio > 0);
	assert(_correction == 1);

	auto scaled = copy_scaled(ratio);
	return EvaluateRactangleCompact(scaled._x1, scaled._y1, scaled._x2, scaled._y2, scaled._corrected_weight);
}