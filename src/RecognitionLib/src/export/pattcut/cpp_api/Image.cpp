#include "inc/export/pattcut/cpp_api/Image.h"
#include "inc/images/Image.h"
#include "inc/images/ImageLoader.h"
#include "inc/support/Exceptions.h" 

namespace PattCutLib
{
	PattCutLib::Image::Image()
	{
		_image_ptr = nullptr;
	}

	PattCutLib::Image::Image(int width, int height)
	{
		if (width <= 0)
			throw ImageOperationException("Image should have positive width");
		if (height <= 0)
			throw ImageOperationException("Image should have positive height");

		_image_ptr = new ::Image(width, height);
	}

	PattCutLib::Image::Image(const char* file_name)
	{
		if (file_name == nullptr)
			throw ImageOperationException("file_name cannot be nullptr");

		_image_ptr = nullptr;
		auto tmpImagePtr = new ::Image();

		try
		{
			*tmpImagePtr = std::move(ImageLoader::load(std::string(file_name)));
			_image_ptr = tmpImagePtr;
		}
		catch (ImageLoadingException& exc)
		{
			delete tmpImagePtr;
			throw ImageOperationException(exc.what());
		}
		catch (...)
		{
			delete tmpImagePtr;
			throw;
		}
	}

	PattCutLib::Image::Image(const std::string& file_name)
	{
		_image_ptr = nullptr;
		auto tmpImagePtr = new ::Image();

		try
		{
			*tmpImagePtr = std::move(ImageLoader::load(file_name));
			_image_ptr = tmpImagePtr;
		}
		catch (ImageLoadingException& exc)
		{
			delete tmpImagePtr;
			throw ImageOperationException(exc.what());
		}
		catch (...)
		{
			delete tmpImagePtr;
			throw;
		}
	}


	PattCutLib::Image::Image(const void* image_bytes, size_t image_bytes_length)
	{
		_image_ptr = nullptr;
		auto tmpImagePtr = new ::Image();

		try
		{
			*tmpImagePtr = std::move(ImageLoader::load(image_bytes, image_bytes_length));
			_image_ptr = tmpImagePtr;
		}
		catch (ImageLoadingException& exc)
		{
			delete tmpImagePtr;
			throw ImageOperationException(exc.what());
		}
		catch (...)
		{
			delete tmpImagePtr;
			throw;
		}
	}


	PattCutLib::Image::~Image()
	{
		if (_image_ptr == nullptr)
			return;

		delete (::Image*)_image_ptr;
		_image_ptr = nullptr;
	}


	PattCutLib::Image::Image(const Image& source)
	{
		if (source._image_ptr == nullptr)
			this->_image_ptr = nullptr;
		else
			this->_image_ptr = new ::Image(*((::Image*)source._image_ptr));
	}

	PattCutLib::Image::Image(Image&& source)
	{
		this->_image_ptr = source._image_ptr;
		source._image_ptr = nullptr;
	}


	Image& PattCutLib::Image::operator=(const Image& source)
	{
		if (this != &source)
		{
			if (source._image_ptr == nullptr)
				this->_image_ptr = nullptr;
			else
				this->_image_ptr = new ::Image(*((::Image*)source._image_ptr));
		}

		return *this;
	}

	Image& PattCutLib::Image::operator=(Image&& source)
	{
		if (this != &source)
		{
			this->_image_ptr = source._image_ptr;
			source._image_ptr = nullptr;
		}

		return *this;
	}



	int PattCutLib::Image::get_width() const
	{
		if (_image_ptr == nullptr)
			return 0;

		return ((::Image*)_image_ptr)->get_width();
	}
	int PattCutLib::Image::get_height() const
	{
		if (_image_ptr == nullptr)
			return 0;

		return ((::Image*)_image_ptr)->get_height();
	}


	unsigned char PattCutLib::Image::get_pixel(int x, int y) const
	{
		if (_image_ptr == nullptr)
			throw ImageOperationException("Cannot get_pixel on empty image");
		if (x < 0)
			throw ImageOperationException("Pixel 'x' coordinate cannot be negative");
		if (y < 0)
			throw ImageOperationException("Pixel 'y' coordinate cannot be negative");
		
		auto image_ptr = (::Image*)_image_ptr;

		if (x >= image_ptr->get_width())
			throw ImageOperationException("Pixel 'x' coordinate cannot be larger than width of image");
		if (y >= image_ptr->get_height())
			throw ImageOperationException("Pixel 'y' coordinate cannot be larger than height of image");

		return image_ptr->get(x, y);
	}
	void PattCutLib::Image::set_pixel(int x, int y, unsigned char value)
	{
		if (_image_ptr == nullptr)
			throw ImageOperationException("Cannot set_pixel on empty image");
		if (x < 0)
			throw ImageOperationException("Pixel 'x' coordinate cannot be negative");
		if (y < 0)
			throw ImageOperationException("Pixel 'y' coordinate cannot be negative");

		auto image_ptr = (::Image*)_image_ptr;

		if (x >= image_ptr->get_width())
			throw ImageOperationException("Pixel 'x' coordinate cannot be larger than width of image");
		if (y >= image_ptr->get_height())
			throw ImageOperationException("Pixel 'y' coordinate cannot be larger than height of image");

		image_ptr->set(x, y, value);
	}
}