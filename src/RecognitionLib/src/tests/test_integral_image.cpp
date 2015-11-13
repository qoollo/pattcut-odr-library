#include "inc/tests/test_integral_image.h"
#include <iostream>
#include "inc/images/IntegralImage.h"

void test_integral_image()
{
	const int w = 3;
	const int h = 5;
	const int value = 2;

	std::shared_ptr<Image> image_pointer(new Image(w, h));

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
			image_pointer->set(i, j, value);
	}
	IntegralImage i_i(image_pointer);
	for (int i = 0; i < w + 1; i++)
	{
		for (int j = 0; j < h + 1; j++)
			std::cout << i_i.get_sum_in_rectangle(0, 0, i, j) << ", ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < w + 1; i++)
	{
		for (int j = 0; j < h + 1; j++)
			std::cout << i_i.get_square_sum_in_rectangle(0, 0, i, j) << ", ";
		std::cout << std::endl;
	}
	std::cout << "test end";
}