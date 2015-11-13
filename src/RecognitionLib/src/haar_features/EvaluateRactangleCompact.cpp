#include "inc/haar_features/EvaluateRactangleCompact.h"
#include <math.h>

EvaluateRactangleCompact::EvaluateRactangleCompact()
{
	_x1 = 0;
	_y1 = 0;
	_x2 = 0;
	_y2 = 0;

	_corrected_weight = 0;
}

EvaluateRactangleCompact::EvaluateRactangleCompact(short x1, short y1, short x2, short y2, float corrected_weight)
{
	assert(x1 >= 0);
	assert(y1 >= 0);
	assert(x2 >= 0);
	assert(y2 >= 0);
	assert(x2 >= x1);
	assert(y2 >= y1);
	assert(corrected_weight != 0);

	_x1 = x1;
	_y1 = y1;
	_x2 = x2;
	_y2 = y2;

	_corrected_weight = corrected_weight;
}

EvaluateRactangleCompact::EvaluateRactangleCompact(int x1, int y1, int x2, int y2, double corrected_weight)
{
	assert(x1 >= 0);
	assert(x1 < std::numeric_limits<short>::max());
	assert(y1 >= 0);
	assert(y1 < std::numeric_limits<short>::max());
	assert(x2 >= 0);
	assert(x2 < std::numeric_limits<short>::max());
	assert(y2 >= 0);
	assert(y2 < std::numeric_limits<short>::max());
	assert(x2 >= x1);
	assert(y2 >= y1);
	assert(corrected_weight != 0);
	assert(corrected_weight < std::numeric_limits<float>::max());

	_x1 = (short)x1;
	_y1 = (short)y1;
	_x2 = (short)x2;
	_y2 = (short)y2;

	_corrected_weight = (float)corrected_weight;
}