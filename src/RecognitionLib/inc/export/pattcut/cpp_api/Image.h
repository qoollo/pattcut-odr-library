#pragma once
#include "pattcut/export_helper.h"
#include <stdexcept>
#include <string>


namespace PattCutLib
{
	/** Exception indicating some error during operation on the Image object */
	class ImageOperationException : public std::runtime_error
	{
	public:
		/** 
		 * ImageOperationException constructor
		 * @error - error description
		 */
		ImageOperationException(const std::string& error) : std::runtime_error(error) { }
	};


	/** Represents grayscale Image object */
	class DLL_EXPORT Image
	{
	public:
		/** Empty image constructor */
		Image();
		/**
		 * Image constructor, that creates image with specified width and height
		 * @width - image width in pixels
		 * @height - image height in pixels
		 */
		Image(int width, int height);
		/**
		 * Image constructor, that creates image from file
		 * @file_name - path to the image file (supported formats: JPEG, PNG, BMP)
		 * @throws ImageOperationException - file not found or file malformed
		 */
		Image(const char* file_name);
		/**
		 * Image constructor, that creates image from file
		 * @file_name - path to the image file (supported formats: JPEG, PNG, BMP)
		 * @throws ImageOperationException - file not found or file malformed
		 */
		Image(const std::string& file_name);
		/**
		 * Image constructor, that creates image from file
		 * @image_bytes - array with image data (supported formats: JPEG, PNG, BMP)
		 * @image_bytes_length - length of the 'image_bytes' array
		 * @throws ImageOperationException - image malformed
		 */
		Image(const void* image_bytes, size_t image_bytes_length);
		/** Image destructor */
		~Image();

		/** Image copy constructor */
		Image(const Image& source);
		/** Image move constructor */
		Image(Image&& source);
		/** Image copy operator */
		Image& operator=(const Image& source);
		/** Image move operator */
		Image& operator=(Image&& source);

		/** Returns image width in pixels */
		int get_width() const;
		/** Returns image height in pixels */
		int get_height() const;

		/**
		 * Gets pixel value at specified coordinates
		 * @x - X coordinate of the pixel
		 * @y - Y coordinate of the pixel
		 * @return - pixel value (intensity 0-255)
		 * @throws ImageOperationException - incorrect coordinates
		 */
		unsigned char get_pixel(int x, int y) const;
		/**
	 	 * Sets pixel value at specified coordinates
		 * @x - X coordinate of the pixel
		 * @y - Y coordinate of the pixel
		 * @value - new pixel value (intensity 0-255)
		 * @throws ImageOperationException - incorrect coordinates
		 */
		void set_pixel(int x, int y, unsigned char value);

	private:
		void* _image_ptr;
	};
}