#ifndef _PATTCUT_RECOGNITION_FUNCTIONS_C_H_
#define _PATTCUT_RECOGNITION_FUNCTIONS_C_H_

#include "pattcut/export_helper.h"
#include "pattcut/c_api/Common.h"
#include "pattcut/c_api/RecognitionResults.h"
#include "pattcut/c_api/Image.h"


/** Represents controller that hold all classifiers and perform recognition */
typedef void* PattCut_RecognitionController;

#define PattCut_RecognitionController_Invalid (0)

#define PattCut_ErrorCode_RecognitionResultCorrupted (-2)


/** Classifier information 
 * @id - Registered identifier for the loaded classifier (used in PattCut_RecognitionResult). Should be positive. Negative value indicate loading error.
 * @window_width - Scanning window width
 * @window_height - Scanning window height
 */
PACK(
struct PattCut_ClassifierInformation
{
	/** Registered identifier for the loaded classifier (used in PattCut_RecognitionResult).
	 * Should be positive. Negative value indicate loading error.
	 */
	int id;

	/** Scanning window width */
	int window_width; 

	/** Scanning window height */
	int window_height;
});


extern "C"
{
	/** 
	 * Creates RecognitionContoller 
	 * @return - Initialized RecognitionController 
	 */
	DLL_EXPORT PattCut_RecognitionController pattcut_init_controller();
	/** 
	 * Destroy created RecognitionContoller
	 * @pController - pointer to the variable with PattCut_RecognitionController
	 * @return - PattCut_ErrorCode_OK if there's no errors, otherwise returns PattCut_ErrorCode_Error
	 */
	DLL_EXPORT PattCut_ErrorCode pattcut_destroy_controller(PattCut_RecognitionController* pController);

	/**
	 * Loads classifier from string containing classifier data in xml format (support native PattCut classifiers and OpenCV cascade classifiers)
	 * @controller - initialized Recognition Controller
	 * @classifier_xml - string that contains xml classifier data to load
	 * @error_message_ptr - pointer to the string to store error description (can be NULL)
	 * @return - information about loaded classifier (its 'id' and other parameters)
	 */
	DLL_EXPORT PattCut_ClassifierInformation pattcut_load_classifier(PattCut_RecognitionController controller, const char* classifier_xml, char** error_message_ptr);
	/**
	* Loads classifier from file in xml format (support native PattCut classifiers and OpenCV cascade classifiers)
	* @controller - initialized Recognition Controller
	* @classifier_file_name - path to the classifier xml file
	* @error_message_ptr - pointer to the string to store error description (can be NULL)
	* @return - information about loaded classifier (its 'id' and other parameters)
	*/
	DLL_EXPORT PattCut_ClassifierInformation pattcut_load_classifier_from_file(PattCut_RecognitionController controller, const char* classifier_file_name, char** error_message_ptr);

	/**
	 * Performs object detection and recognition for image by all classifiers loaded to Recognition Controller
	 * @controller - initialized Recognition Controller
	 * @image_bytes - array with image data (supported formats: JPEG, PNG, BMP)
	 * @image_bytes_length - length of the 'image_bytes' array
	 * @error_message_ptr - pointer to the string to store error description (can be NULL)
	 * @return - results of the recognition process 
	 */
	DLL_EXPORT PattCut_RecognitionResult* pattcut_perform_recognition(PattCut_RecognitionController controller, const void* image_bytes, unsigned int image_bytes_length, char** error_message_ptr);
	/**
	* Performs object detection and recognition on Image passed as image object with specified classifier ids.
	* @controller - initialized Recognition Controller
	* @image - loaded image
	* @classifier_ids -  array with classifier identifiers to process current image (NULL means all classifiers should be used)
	* @classifier_ids_length - length of the 'classifier_ids' array
	* @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
	* @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
	* @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
	* @step_factor - scanning window step (-1 - means 1 pixel by default)
	* @error_message_ptr - pointer to the string to store error description (can be NULL)
	* @return - results of the recognition process
	*/
	DLL_EXPORT PattCut_RecognitionResult* pattcut_perform_recognition_on_image(PattCut_RecognitionController controller, PattCut_Image image, const int* classifier_ids, unsigned int classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor, char** error_message_ptr);
	/**
	* Performs object detection and recognition on image with specified classifier ids.
	* @controller - initialized Recognition Controller
	* @image_bytes - array with image data
	* @image_bytes_length - length of the 'image_bytes' array
	* @classifier_ids -  array with classifier identifiers to process current image (NULL means all classifiers should be used)
	* @classifier_ids_length - length of the 'classifier_ids' array
	* @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
	* @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
	* @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
	* @step_factor - scanning window step (-1 - means 1 pixel by default)
	* @error_message_ptr - pointer to the string to store error description (can be NULL)
	* @return - results of the recognition process
	*/
	DLL_EXPORT PattCut_RecognitionResult* pattcut_perform_recognition_on_image_bytes(PattCut_RecognitionController controller, const void* image_bytes, unsigned int image_bytes_length, const int* classifier_ids, unsigned int classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor, char** error_message_ptr);
	/**
	* Performs object detection and recognition on image file with specified classifier ids.
	* @controller - initialized Recognition Controller
	* @file_name - path to the image file on disk (supported formats: JPEG, PNG, BMP)
	* @classifier_ids -  array with classifier identifiers to process current image (NULL means all classifiers should be used)
	* @classifier_ids_length - length of the 'classifier_ids' array
	* @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
	* @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
	* @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
	* @step_factor - scanning window step (-1 - means 1 pixel by default)
	* @error_message_ptr - pointer to the string to store error description (can be NULL)
	* @return - results of the recognition process
	*/
	DLL_EXPORT PattCut_RecognitionResult* pattcut_perform_recognition_on_image_file(PattCut_RecognitionController controller, const char* image_file_name, const int* classifier_ids, unsigned int classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor, char** error_message_ptr);

	/**
	 * Free memory allocated for PattCut_RecognitionResult
	 * @pResults - pointer to the variable containing 'PattCut_RecognitionResult*' returned from one of the 'pattcut_perform_recognition_*' functions
	 * @return - PattCut_ErrorCode_OK if there's no errors, otherwise returns PattCut_ErrorCode_Error
	 */
	DLL_EXPORT PattCut_ErrorCode pattcut_free_recognition_results(PattCut_RecognitionResult** pResults);
}

#endif