#pragma once
#include "pattcut/export_helper.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "pattcut/cpp_api/RecognitionResults.h"
#include "pattcut/cpp_api/Image.h"

namespace PattCutLib
{
	/** Exception indicates some error during operation with RecognitionController */
	class RecognitionException : public std::runtime_error
	{
	public:
		/** 
		 * RecognitionException constructor
		 * @error - error description
		 */
		RecognitionException(const std::string& error) : std::runtime_error(error) { }
	};


	/** Classifier information  */
	class ClassifierInformation
	{
	public:
		/** ClassifierInformation constructor */
		ClassifierInformation()
		{
			_id = 0;
			_window_width = 0;
			_window_height = 0;
		}
		/**
		 * ClassifierInformation constructor
		 * @id - classifier identifier
		 * @window_width - scanning window width
		 * @window_height - scanning window height
		 */
		ClassifierInformation(int id, int window_width, int window_height)
		{
			_id = id;
			_window_width = window_width;
			_window_height = window_height;
		}

		/** Gets identifier for the loaded classifier (used in PattCutLib::RecognitionResult). Should be positive. */
		int id() const { return _id; }
		/** Gets scanning window width */
		int window_width() const { return _window_width; }
		/** Gets scanning window height */
		int window_height() const { return _window_height; }

	private:
		int _id;
		int _window_width;
		int _window_height;
	};


	/** Controller that hold all classifiers and perform recognition */
	class DLL_EXPORT RecognitionController
	{
	public:
		/** RecognitionController constructor */
		RecognitionController();
		/** RecognitionController destructor */
		~RecognitionController();

		/** RecognitionController cannot be copied */
		RecognitionController(const RecognitionController&) = delete;
		/** RecognitionController cannot be copied */
		RecognitionController& operator=(const RecognitionController&) = delete;


		/**
		 * Loads classifier from string containing classifier data in xml format (support native PattCut classifiers and OpenCV cascade classifiers)
		 * @classifier_xml - string that contains xml classifier data to load
		 * @return - information about loaded classifier (its 'id' and other parameters)
		 * @throws RecognitionException - malformed classifier
		 */
		ClassifierInformation load_classifier(const char* classifier_xml);
		/**
		 * Loads classifier from string containing classifier data in xml format (support native PattCut classifiers and OpenCV cascade classifiers)
		 * @classifier_xml - string that contains xml classifier data to load
		 * @return - information about loaded classifier (its 'id' and other parameters)
		 * @throws RecognitionException - malformed classifier
		 */
		ClassifierInformation load_classifier(const std::string& classifier_xml);

		/**
		* Loads classifier from file in xml format (support native PattCut classifiers and OpenCV cascade classifiers)
		* @classifier_file_name - path to the classifier xml file
		* @return - information about loaded classifier (its 'id' and other parameters)
		* @throws RecognitionException - file not found or malformed classifier
		*/
		ClassifierInformation load_classifier_from_file(const char* classifier_file_name);
		/**
		 * Loads classifier from file in xml format (support native PattCut classifiers and OpenCV cascade classifiers)
		 * @classifier_file_name - path to the classifier xml file
		 * @return - information about loaded classifier (its 'id' and other parameters)
		 * @throws RecognitionException - file not found or malformed classifier
		 */
		ClassifierInformation load_classifier_from_file(const std::string& classifier_file_name);


		/**
		 * Performs object detection and recognition on Image passed as image object
		 * @image - loaded image
		 * @return - results of the recognition process
		 * @throws RecognitionException - some error during recognition
		 */
		RecognitionResult perform_recognition(const Image& image) const;
		/**
		 * Performs object detection and recognition on Image passed as image object with specified classifier ids
		 * @image - loaded image
		 * @classifier_ids - vector with classifier identifiers to process current image (empty means all classifiers should be used)
		 * @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
		 * @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
		 * @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
		 * @step_factor - scanning window step (-1 - means 1 pixel by default)
		 * @return - results of the recognition process
		 * @throws RecognitionException - some error during recognition
		 */
		RecognitionResult perform_recognition(const Image& image, const std::vector<int>& classifier_ids, int min_object_size = -1, int max_object_size = -1, double scale_factor = -1, double step_factor = -1) const;	

		/**
		 * Performs object detection and recognition on image file by all classifiers loaded to Recognition Controller
		 * @image_file_name - path to the image file on disk (supported formats: JPEG, PNG, BMP)
		 * @return - results of the recognition process
		 * @throws RecognitionException - image file not found, image file malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const std::string& image_file_name) const;
		/**
		 * Performs object detection and recognition on image file with specified classifier ids
		 * @image_file_name - path to the image file on disk (supported formats: JPEG, PNG, BMP)
		 * @classifier_ids - vector with classifier identifiers to process current image (empty means all classifiers should be used)
		 * @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
		 * @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
		 * @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
		 * @step_factor - scanning window step (-1 - means 1 pixel by default)
		 * @return - results of the recognition process
		 * @throws RecognitionException - image file not found, image file malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const std::string& image_file_name, const std::vector<int>& classifier_ids, int min_object_size = -1, int max_object_size = -1, double scale_factor = -1, double step_factor = -1) const;

		/**
		 * Performs object detection and recognition on image file by all classifiers loaded to Recognition Controller
		 * @image_file_name - path to the image file on disk (supported formats: JPEG, PNG, BMP)
		 * @return - results of the recognition process
		 * @throws RecognitionException - image file not found, image file malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const char* image_file_name) const;
		/**
		 * Performs object detection and recognition on image file with specified classifier ids
		 * @image_file_name - path to the image file on disk (supported formats: JPEG, PNG, BMP)
		 * @classifier_ids - array with classifier identifiers to process current image (NULL means all classifiers should be used)
		 * @classifier_ids_length - length of 'classifier_ids' array
		 * @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
		 * @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
		 * @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
		 * @step_factor - scanning window step (-1 - means 1 pixel by default)
		 * @return - results of the recognition process
		 * @throws RecognitionException - image file not found, image file malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const char* image_file_name, const int* classifier_ids, size_t classifier_ids_length, int min_object_size = -1, int max_object_size = -1, double scale_factor = -1, double step_factor = -1) const;

		/**
		 * Performs object detection and recognition for image by all classifiers loaded to Recognition Controller
		 * @image_bytes - array with image data (supported formats: JPEG, PNG, BMP)
		 * @image_bytes_length - length of the 'image_bytes' array
		 * @return - results of the recognition process
		 * @throws RecognitionException - image malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const void* image_bytes, size_t image_bytes_length) const;
		/**
		 * Performs object detection and recognition for image by specified classifier ids
		 * @image_bytes - array with image data (supported formats: JPEG, PNG, BMP)
		 * @image_bytes_length - length of the 'image_bytes' array
		 * @classifier_ids - vector with classifier identifiers to process current image (empty means all classifiers should be used)
		 * @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
		 * @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
		 * @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
		 * @step_factor - scanning window step (-1 - means 1 pixel by default)
		 * @return - results of the recognition process
		 * @throws RecognitionException - image malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const void* image_bytes, size_t image_bytes_length, const std::vector<int>& classifier_ids, int min_object_size = -1, int max_object_size = -1, double scale_factor = -1, double step_factor = -1) const;
		/**
		 * Performs object detection and recognition for image by specified classifier ids
		 * @image_bytes - array with image data (supported formats: JPEG, PNG, BMP)
		 * @image_bytes_length - length of the 'image_bytes' array
		 * @classifier_ids - array with classifier identifiers to process current image (NULL means all classifiers should be used)
		 * @classifier_ids_length - length of 'classifier_ids' array
		 * @min_object_size - minimum object size to detect (-1 - automatically choose minimum possible size)
		 * @max_object_size - maximum object size to detect (-1 - automatically choose maximum possible size)
		 * @scale_factor - specifying how much the image size is reduced at each image scale. Should be greater than 1 (-1 - means default value)
		 * @step_factor - scanning window step (-1 - means 1 pixel by default)
		 * @return - results of the recognition process
		 * @throws RecognitionException - image malformed or some error during recognition happened
		 */
		RecognitionResult perform_recognition(const void* image_bytes, size_t image_bytes_length, const int* classifier_ids, size_t classifier_ids_length, int min_object_size = -1, int max_object_size = -1, double scale_factor = -1, double step_factor = -1) const;

	private:
		int _last_classifier_id;
		void* _multi_class_viola_jones_controller;
	};
}

