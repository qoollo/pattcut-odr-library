#pragma once
#include "inc/images/Image.h"
#include "inc/support/Exceptions.h"

class ImageLoader
{
public:
	static Image load(const void* data, const size_t data_length);
	static Image load(const std::string& file_name);

	static Image load_with_image_magic(const void* data, const size_t data_length);

	static size_t save_bmp(const Image& image, void* buffer, const size_t buffer_length);

private:
	static Image load_jpeg(const void* data, const size_t data_length);
	static Image load_png(const void* data, const size_t data_length);
	static Image load_bmp(const void* data, const size_t data_length);
};