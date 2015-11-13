#ifndef _PATTCUT_IMAGE_H_
#define _PATTCUT_IMAGE_H_

#include "pattcut/export_helper.h"
#include "pattcut/c_api/Common.h"

/** Represents grayscale Image object */
typedef void* PattCut_Image;

#define PattCut_Image_Invalid (0)

extern "C"
{
	/** 
	 * Loads image from byte array.
	 * Array should contain image data in the one of the following format: JPEG, PNG, BMP.
	 * @image_bytes - array with image data
	 * @image_bytes_length - length of the 'image_bytes' array
	 * @error_message_ptr - pointer to the string to store error description (can be NULL)
	 * @return - loaded Image or NULL if the error occurred
	 */
	DLL_EXPORT PattCut_Image pattcut_load_image(const void* image_bytes, unsigned int image_bytes_length, char** error_message_ptr);
	/** 
	 * Loads image from file. Supported formats: JPEG, PNG, BMP.
	 * @file_name - path to the image file on disk
	 * @error_message_ptr - pointer to the string to store error description (can be NULL)
	 * @return - loaded Image or NULL if the error occurred
	 */
	DLL_EXPORT PattCut_Image pattcut_load_image_from_file(const char* file_name, char** error_message_ptr);
	/** 
	 * Destroy loaded image object.
	 * @image - loaded PattCut_Image
	 * @return - PattCut_ErrorCode_OK if there's no errors, otherwise returns PattCut_ErrorCode_Error
	 */
	DLL_EXPORT PattCut_ErrorCode pattcut_destroy_image(PattCut_Image* image);

	/** 
	 * Gets the image width in pixels
	 * @image - loaded image object
	 * @return - width of the image
	 */
	DLL_EXPORT int pattcut_get_image_width(PattCut_Image image);
	/** 
	 * Gets the image height in pixels
	 * @image - loaded image object
	 * @return - height of the image
	 */
	DLL_EXPORT int pattcut_get_image_height(PattCut_Image image);
}


#endif