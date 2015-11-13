#define _CRT_SECURE_NO_WARNINGS
#include "inc/export/pattcut/c_api/RecognitionFunctions.h"
#include <string.h>
#include "inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesController.h"
#include "inc/support/Exceptions.h"
#include "inc/classifier_loaders/ClassifierLoader.h"
#include "inc/images/Image.h"
#include "inc/images/ImageLoader.h"


class RecognitionControllerClass
{
public:
	RecognitionControllerClass() : last_classifier_id(1) { }
public:
	int last_classifier_id;
	MultiClassViolaJonesController controller;
};


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


PattCut_RecognitionController pattcut_init_controller()
{
	try
	{
		return new RecognitionControllerClass();
	}
	catch (...)
	{
		return nullptr;
	}
}
PattCut_ErrorCode pattcut_destroy_controller(PattCut_RecognitionController* pController)
{
	if (pController == nullptr) 
		return PattCut_ErrorCode_Error;
	if (*pController == nullptr)
		return PattCut_ErrorCode_OK;

	try
	{
		RecognitionControllerClass* real_controller = (RecognitionControllerClass*)(*pController);
		delete real_controller;
		*pController = nullptr;
		return PattCut_ErrorCode_OK;
	}
	catch (...)
	{
		return PattCut_ErrorCode_Error;
	}
}


PattCut_ClassifierInformation pattcut_load_classifier(PattCut_RecognitionController controller, const char* classifier_xml, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (controller == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'controller' cannot be null");
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}
	if (classifier_xml == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'classifier_xml' cannot be null");
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}

	try
	{
		RecognitionControllerClass* real_controller = (RecognitionControllerClass*)controller;
		PattCut_ClassifierInformation result;
		int my_id = real_controller->last_classifier_id++;

		auto classifier = ClassifierLoader::load_from_xml_string(classifier_xml, ClassificationTag(my_id), ClassificationTag(0));
		result.id = my_id;
		result.window_width = classifier.get_wnd_width();
		result.window_height = classifier.get_wnd_height();

		real_controller->controller.add_recognizer(my_id, std::move(classifier));
		return result;
	}
	catch (std::exception& exc)
	{
		init_error_message_buffer(error_message_ptr, exc.what());
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}
	catch (...)
	{
		init_error_message_buffer(error_message_ptr, "Unknown error");
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}
}

PattCut_ClassifierInformation pattcut_load_classifier_from_file(PattCut_RecognitionController controller, const char* classifier_file_name, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (controller == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'controller' cannot be null");
		PattCut_ClassifierInformation result; 
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}
	if (classifier_file_name == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'classifier_file_name' cannot be null");
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}

	try
	{
		RecognitionControllerClass* real_controller = (RecognitionControllerClass*)controller;
		PattCut_ClassifierInformation result;
		int my_id = real_controller->last_classifier_id++;

		auto classifier = ClassifierLoader::load_from_file(classifier_file_name, ClassificationTag(my_id), ClassificationTag(0));
		result.id = my_id;
		result.window_width = classifier.get_wnd_width();
		result.window_height = classifier.get_wnd_height();

		real_controller->controller.add_recognizer(my_id, std::move(classifier));
		return result;
	}
	catch (std::exception& exc)
	{
		init_error_message_buffer(error_message_ptr, exc.what());
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}
	catch (...)
	{
		init_error_message_buffer(error_message_ptr, "Unknown error");
		PattCut_ClassifierInformation result;
		memset(&result, 0, sizeof(PattCut_ClassifierInformation));
		result.id = PattCut_ErrorCode_Error;
		return result;
	}
}









PattCut_RecognitionResult* pattcut_convert_results(const MultiClassViolaJonesRecognitionResult& source)
{
	PattCut_RecognitionResult* result = new PattCut_RecognitionResult();
	memset(result, 0, sizeof(PattCut_RecognitionResult));
	result->best_classifier_id = source.get_best_classif_id();
	result->total_time_ms = source.get_overall_time_ms();
	result->result_per_classifier_count = (int)source.get_overall_results().size();
	result->result_per_classifier = nullptr;

	if (result->best_classifier_id < 0)
		result->best_classifier_id = 0;

	if (result->result_per_classifier_count > 0)
	{
		result->result_per_classifier = new PattCut_ResultPerClassifier[result->result_per_classifier_count];
		memset(result->result_per_classifier, 0, sizeof(PattCut_ResultPerClassifier) * result->result_per_classifier_count);

		for (int cls_id = 0; cls_id < result->result_per_classifier_count; cls_id++)
		{
			auto* result_per_cls = result->result_per_classifier + cls_id;
			auto& src_result_per_cls = source.get_overall_results().at(cls_id);

			result_per_cls->classifier_id = src_result_per_cls.get_class_id();
			result_per_cls->recognition_time_ms = src_result_per_cls.get_rec_time_ms();
			result_per_cls->window_count = src_result_per_cls.get_wnd_count();
			result_per_cls->window_count_after_prefilter = src_result_per_cls.get_wnd_after_prefilter();
			result_per_cls->mean_depth = (float)src_result_per_cls.get_mean_depth();
			result_per_cls->mean_weak_cls_count = (float)src_result_per_cls.get_mean_weak_count();
			result_per_cls->areas_count = (int)src_result_per_cls.get_areas_info().size();
			result_per_cls->areas = nullptr;

			if (result_per_cls->areas_count > 0)
			{
				result_per_cls->areas = new PattCut_CapturedArea[result_per_cls->areas_count];
				memset(result_per_cls->areas, 0, sizeof(PattCut_CapturedArea) * result_per_cls->areas_count);

				for (int area_id = 0; area_id < result_per_cls->areas_count; area_id++)
				{
					auto* area = result_per_cls->areas + area_id;
					auto& src_area = src_result_per_cls.get_areas_info().at(area_id);

					area->is_good = src_area.get_is_good();
					area->veracity = (float)src_area.get_accuracy();
					area->rect_x = src_area.get_inner_area_info().get_area().left();
					area->rect_y = src_area.get_inner_area_info().get_area().top();
					area->rect_width = src_area.get_inner_area_info().get_area().width();
					area->rect_height = src_area.get_inner_area_info().get_area().height();
					area->merged_window_count = src_area.get_inner_area_info().get_wnd_count();
					area->combined_strong_cls_output = (float)src_area.get_inner_area_info().get_comb_boosted_classifier_value_max();
				}
			}
		}
	}

	return result;
}

PattCut_ErrorCode pattcut_free_recognition_results(PattCut_RecognitionResult** pResults)
{
	if (pResults == nullptr)
		return PattCut_ErrorCode_Error;
	if (*pResults == nullptr)
		return PattCut_ErrorCode_OK;

	PattCut_ErrorCode res_code = PattCut_ErrorCode_OK;
	PattCut_RecognitionResult* results = *pResults;

	try
	{
		if (results->result_per_classifier != nullptr)
		{
			for (int i = 0; i < results->result_per_classifier_count; i++)
			{
				auto* result_per_cls = results->result_per_classifier + i;

				if (result_per_cls->areas != nullptr)
				{
					delete[] result_per_cls->areas;
					result_per_cls->areas = nullptr;
				}
				else
				{
					if (result_per_cls->areas_count != 0)
						res_code = PattCut_ErrorCode_RecognitionResultCorrupted;
				}
			}

			delete[] results->result_per_classifier;
			results->result_per_classifier = nullptr;
		}
		else
		{
			if (results->result_per_classifier_count != 0)
				res_code = PattCut_ErrorCode_RecognitionResultCorrupted;
		}

		memset(results, 0, sizeof(PattCut_RecognitionResult));
		delete results;
		*pResults = nullptr;
	}
	catch (...)
	{
		res_code = PattCut_ErrorCode_Error;
	}

	return res_code;
}






PattCut_RecognitionResult* pattcut_perform_recognition(PattCut_RecognitionController controller, const void* image_bytes, unsigned int image_bytes_length, char** error_message_ptr)
{
	return pattcut_perform_recognition_on_image_bytes(controller, image_bytes, image_bytes_length, nullptr, 0, -1, -1, -1.0, -1.0, error_message_ptr);
}

PattCut_RecognitionResult* pattcut_perform_recognition_on_image(PattCut_RecognitionController controller, PattCut_Image image, const int* classifier_ids, unsigned int classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (controller == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'controller' cannot be null");
		return nullptr;
	}
	if (image == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'image' cannot be null");
		return nullptr;
	}
	if (classifier_ids_length < 0)
	{
		init_error_message_buffer(error_message_ptr, "'classifier_ids_length' cannot be negative");
		return nullptr;
	}
	if (scale_factor >= 0 && scale_factor <= 1.0)
	{
		init_error_message_buffer(error_message_ptr, "'scale_factor' should be greater than 1.0 (or less than 0 to use as default value)");
		return nullptr;
	}


	try
	{
		RecognitionControllerClass* real_controller = (RecognitionControllerClass*)controller;
		::Image* real_image = (::Image*)image;
		std::vector<int> classifier_ids_vector;
		if (classifier_ids != nullptr && classifier_ids_length > 0)
		{
			classifier_ids_vector.reserve(classifier_ids_length);
			for (size_t i = 0; i < classifier_ids_length; i++)
				classifier_ids_vector.push_back((int)classifier_ids[i]);
		}
		auto rec_result = real_controller->controller.recognize(*real_image, classifier_ids_vector, min_object_size, max_object_size, scale_factor, step_factor);
		return pattcut_convert_results(rec_result);
	}
	catch (std::exception& exc)
	{
		init_error_message_buffer(error_message_ptr, exc.what());
		return nullptr;
	}
	catch (...)
	{
		init_error_message_buffer(error_message_ptr, "Unknown error");
		return nullptr;
	}
}


PattCut_RecognitionResult* pattcut_perform_recognition_on_image_bytes(PattCut_RecognitionController controller, const void* image_bytes, unsigned int image_bytes_length, const int* classifier_ids, unsigned int classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (controller == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'controller' cannot be null");
		return nullptr;
	}
	if (image_bytes == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'image_bytes' cannot be null");
		return nullptr;
	}
	if (image_bytes_length < 0)
	{
		init_error_message_buffer(error_message_ptr, "'image_bytes_length' cannot be negative");
		return nullptr;
	}
	if (classifier_ids_length < 0)
	{
		init_error_message_buffer(error_message_ptr, "'classifier_ids_length' cannot be negative");
		return nullptr;
	}
	if (scale_factor >= 0 && scale_factor <= 1.0)
	{
		init_error_message_buffer(error_message_ptr, "'scale_factor' should be greater than 1.0 (or less than 0 to use as default value)");
		return nullptr;
	}


	try
	{
		RecognitionControllerClass* real_controller = (RecognitionControllerClass*)controller;
		::Image real_image = ImageLoader::load(image_bytes, image_bytes_length);
		std::vector<int> classifier_ids_vector;
		if (classifier_ids != nullptr && classifier_ids_length > 0)
		{
			classifier_ids_vector.reserve(classifier_ids_length);
			for (size_t i = 0; i < classifier_ids_length; i++)
				classifier_ids_vector.push_back((int)classifier_ids[i]);
		}
		auto rec_result = real_controller->controller.recognize(real_image, classifier_ids_vector, min_object_size, max_object_size, scale_factor, step_factor);
		return pattcut_convert_results(rec_result);
	}
	catch (std::exception& exc)
	{
		init_error_message_buffer(error_message_ptr, exc.what());
		return nullptr;
	}
	catch (...)
	{
		init_error_message_buffer(error_message_ptr, "Unknown error");
		return nullptr;
	}
}


PattCut_RecognitionResult* pattcut_perform_recognition_on_image_file(PattCut_RecognitionController controller, const char* image_file_name, const int* classifier_ids, unsigned int classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor, char** error_message_ptr)
{
	init_error_message_buffer(error_message_ptr, nullptr);

	if (controller == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'controller' cannot be null");
		return nullptr;
	}
	if (image_file_name == nullptr)
	{
		init_error_message_buffer(error_message_ptr, "'image_file_name' cannot be null");
		return nullptr;
	}
	if (classifier_ids_length < 0)
	{
		init_error_message_buffer(error_message_ptr, "'classifier_ids_length' cannot be negative");
		return nullptr;
	}
	if (scale_factor >= 0 && scale_factor <= 1.0)
	{
		init_error_message_buffer(error_message_ptr, "'scale_factor' should be greater than 1.0 (or less than 0 to use as default value)");
		return nullptr;
	}


	try
	{
		RecognitionControllerClass* real_controller = (RecognitionControllerClass*)controller;
		::Image real_image = ImageLoader::load(image_file_name);
		std::vector<int> classifier_ids_vector;
		if (classifier_ids != nullptr && classifier_ids_length > 0)
		{
			classifier_ids_vector.reserve(classifier_ids_length);
			for (size_t i = 0; i < classifier_ids_length; i++)
				classifier_ids_vector.push_back((int)classifier_ids[i]);
		}
		auto rec_result = real_controller->controller.recognize(real_image, classifier_ids_vector, min_object_size, max_object_size, scale_factor, step_factor);
		return pattcut_convert_results(rec_result);
	}
	catch (std::exception& exc)
	{
		init_error_message_buffer(error_message_ptr, exc.what());
		return nullptr;
	}
	catch (...)
	{
		init_error_message_buffer(error_message_ptr, "Unknown error");
		return nullptr;
	}
}
