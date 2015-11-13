#define _CRT_SECURE_NO_WARNINGS
#include "inc/export/pattcut/c_api/Image.h"
#include "inc/images/Image.h"
#include "inc/images/ImageLoader.h"
#include "inc/support/Exceptions.h" 


static void init_error_message_buffer(char** error_message_ptr, const char* source_msg)
{
	if (error_message_ptr == nullptr)
		return;

	if (source_msg == nullptr)
	{
		*error_message_ptr = nullptr;
		return;
	}

	auto len = strlen(source_msg);
	if (len > 1024)
		len = 1024;
	*error_message_ptr = new char[len + 1];

	strncpy(*error_message_ptr, source_msg, len);
	(*error_message_ptr)[len] = 0;
}



PattCut_Image pattcut_load_image(const void* image_bytes, unsigned int image_bytes_length, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (image_bytes == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'image_bytes' cannot be null");
		return nullptr;
	}
	if (image_bytes_length == 0)
	{
		init_error_message_buffer(error_message_ptr, "'image_bytes_length' cannot be zero");
		return nullptr;
	}


	auto result = new ::Image();

	try
	{
		*result = std::move(ImageLoader::load(image_bytes, image_bytes_length));
		return result;
	}
	catch (std::exception& exc)
	{
		delete result;
		init_error_message_buffer(error_message_ptr, exc.what());
		return nullptr;
	}
	catch (...)
	{
		delete result;
		init_error_message_buffer(error_message_ptr, "Unknown error");
		return nullptr;
	}
}


PattCut_Image pattcut_load_image_from_file(const char* file_name, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (file_name == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'file_name' cannot be null");
		return nullptr;
	}

	auto result = new ::Image();

	try
	{
		*result = std::move(ImageLoader::load(std::string(file_name)));
		return result;
	}
	catch (std::exception& exc)
	{
		delete result;
		init_error_message_buffer(error_message_ptr, exc.what());
		return nullptr;
	}
	catch (...)
	{
		delete result;
		init_error_message_buffer(error_message_ptr, "Unknown error");
		return nullptr;
	}
}



PattCut_ErrorCode pattcut_destroy_image(PattCut_Image* image)
{
	if (image == nullptr)
		return PattCut_ErrorCode_Error;
	if (*image == nullptr)
		return PattCut_ErrorCode_OK;

	try
	{
		::Image* real_image = (::Image*)(*image);
		delete real_image;
		*image = nullptr;
		return PattCut_ErrorCode_OK;
	}
	catch (...)
	{
		return PattCut_ErrorCode_Error;
	}
}


int pattcut_get_image_width(PattCut_Image image)
{
	if (image == nullptr)
		return 0;

	try
	{
		::Image* real_image = (::Image*)image;
		return real_image->get_width();
	}
	catch (...)
	{
		return PattCut_ErrorCode_Error;
	}
}


int pattcut_get_image_height(PattCut_Image image)
{
	if (image == nullptr)
		return 0;

	try
	{
		::Image* real_image = (::Image*)image;
		return real_image->get_height();
	}
	catch (...)
	{
		return PattCut_ErrorCode_Error;
	}
}