#include "inc/export/pattcut/cpp_api/RecognitionController.h"
#include <utility>
#include <atomic>
#include <fstream>
#include "inc/support/Exceptions.h"
#include "inc/classifier_loaders/ClassifierLoader.h"
#include "inc/images/Image.h"
#include "inc/images/ImageLoader.h"
#include "inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesController.h"

namespace PattCutLib
{
	static ::Image* extract_image_ptr(const PattCutLib::Image& image)
	{
		// image ptr is a single filed
		return *((::Image**)(&image));
	}


	RecognitionController::RecognitionController()
	{
		_last_classifier_id = 1;
		_multi_class_viola_jones_controller = new MultiClassViolaJonesController();
	}

	RecognitionController::~RecognitionController()
	{
		MultiClassViolaJonesController* controller = (MultiClassViolaJonesController*)_multi_class_viola_jones_controller;
		if (controller != nullptr)
		{
			delete controller;
			_multi_class_viola_jones_controller = nullptr;
		}
	}



	ClassifierInformation RecognitionController::load_classifier(const std::string& classifier_xml)
	{
		MultiClassViolaJonesController* controller = (MultiClassViolaJonesController*)_multi_class_viola_jones_controller;
		int my_id = _last_classifier_id++;

		try
		{
			auto classifier = ClassifierLoader::load_from_xml_string(classifier_xml, ClassificationTag(my_id), ClassificationTag(0));
			int wnd_width = classifier.get_wnd_width();
			int wnd_height = classifier.get_wnd_height();

			controller->add_recognizer(my_id, std::move(classifier));

			return ClassifierInformation(my_id, wnd_width, wnd_height);
		}
		catch (ClassifierLoadingException& cl_exc)
		{
			throw RecognitionException(std::string("Classifier loading exception: ") + cl_exc.what());
		}
		catch (LibraryException& l_exc)
		{
			throw RecognitionException(std::string("Library exception: ") + l_exc.what());
		}
	}
	ClassifierInformation RecognitionController::load_classifier_from_file(const std::string& classifier_file_name)
	{
		MultiClassViolaJonesController* controller = (MultiClassViolaJonesController*)_multi_class_viola_jones_controller;
		int my_id = _last_classifier_id++;

		try
		{
			auto classifier = ClassifierLoader::load_from_file(classifier_file_name, ClassificationTag(my_id), ClassificationTag(0));
			int wnd_width = classifier.get_wnd_width();
			int wnd_height = classifier.get_wnd_height();

			controller->add_recognizer(my_id, std::move(classifier));
			
			return ClassifierInformation(my_id, wnd_width, wnd_height);
		}
		catch (ClassifierLoadingException& cl_exc)
		{
			throw RecognitionException(std::string("Classifier loading exception: ") + cl_exc.what());
		}
		catch (LibraryException& l_exc)
		{
			throw RecognitionException(std::string("Library exception: ") + l_exc.what());
		}
	}

	ClassifierInformation RecognitionController::load_classifier(const char* classifier_xml)
	{
		if (classifier_xml == nullptr)
			throw RecognitionException("Argument 'classifier_xml' cannot be null");

		return this->load_classifier(std::string(classifier_xml));
	}
	ClassifierInformation RecognitionController::load_classifier_from_file(const char* classifier_file_name)
	{
		if (classifier_file_name == nullptr)
			throw RecognitionException("Argument 'classifier_file_name' cannot be null");

		return this->load_classifier_from_file(std::string(classifier_file_name));
	}




	RecognitionResult transform_results(const MultiClassViolaJonesRecognitionResult& source)
	{
		std::vector<PattCutLib::ResultPerClassifier> result_by_classif;
		result_by_classif.reserve(source.get_overall_results().size());
		for (auto& per_cls_res : source.get_overall_results())
		{
			std::vector<PattCutLib::CapturedArea> areas;
			areas.reserve(per_cls_res.get_areas_info().size());
			for (auto& area : per_cls_res.get_areas_info())
			{
				auto& inner_area_info = area.get_inner_area_info();
				auto& rect = area.get_inner_area_info().get_area();
				areas.push_back(PattCutLib::CapturedArea(rect.left(), rect.top(), rect.width(), rect.height(),
														area.get_accuracy(), area.get_is_good(),
														inner_area_info.get_wnd_count(), inner_area_info.get_comb_boosted_classifier_value_max()));
			}


			result_by_classif.push_back(PattCutLib::ResultPerClassifier(
									per_cls_res.get_class_id(), per_cls_res.get_rec_time_ms(), 
									per_cls_res.get_wnd_count(), per_cls_res.get_wnd_after_prefilter(),
									per_cls_res.get_mean_depth(), per_cls_res.get_mean_weak_count(),
									std::move(areas)));
		}

		int best_classif_id = source.get_best_classif_id();
		if (best_classif_id < 0)
			best_classif_id = 0;

		return RecognitionResult(best_classif_id, source.get_overall_time_ms(), std::move(result_by_classif));
	}


	RecognitionResult RecognitionController::perform_recognition(const PattCutLib::Image& image, const std::vector<int>& classifier_ids, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
	{
		if (scale_factor >= 0 && scale_factor <= 1.0)
			throw RecognitionException("Argument 'scale_factor' should be greater than 1.0 (or less than 0 to use as default value)");

		MultiClassViolaJonesController* controller = (MultiClassViolaJonesController*)_multi_class_viola_jones_controller;
		::Image* image_ptr = extract_image_ptr(image);

		try
		{
			if (image_ptr == nullptr)
				return RecognitionResult();

			auto rec_result = controller->recognize(*image_ptr, classifier_ids, min_object_size, max_object_size, scale_factor, step_factor);
			return transform_results(rec_result);
		}
		catch (LibraryException& l_exc)
		{
			throw RecognitionException(std::string("Library exception: ") + l_exc.what());
		}
	}
	RecognitionResult RecognitionController::perform_recognition(const PattCutLib::Image& image) const
	{
		return perform_recognition(image, std::vector<int>(), -1, -1, -1.0, -1.0);
	}

	RecognitionResult RecognitionController::perform_recognition(const std::string& image_file_name, const std::vector<int>& classifier_ids, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
	{
		if (scale_factor >= 0 && scale_factor <= 1.0)
			throw RecognitionException("Argument 'scale_factor' should be greater than 1.0 (or less than 0 to use as default value)");

		MultiClassViolaJonesController* controller = (MultiClassViolaJonesController*)_multi_class_viola_jones_controller;

		try
		{
			auto image = ImageLoader::load(image_file_name);
			auto rec_result = controller->recognize(image, classifier_ids, min_object_size, max_object_size, scale_factor, step_factor);
			return transform_results(rec_result);
		}
		catch (ImageLoadingException& il_exc)
		{
			throw RecognitionException(std::string("Image loading exception: ") + il_exc.what());
		}
		catch (LibraryException& l_exc)
		{
			throw RecognitionException(std::string("Library exception: ") + l_exc.what());
		}
	}
	RecognitionResult RecognitionController::perform_recognition(const std::string& image_file_name) const
	{
		return this->perform_recognition(image_file_name, std::vector<int>(), -1, -1, -1.0, -1.0);
	}


	RecognitionResult RecognitionController::perform_recognition(const void* image_bytes, size_t image_bytes_length, const std::vector<int>& classifier_ids, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
	{
		if (image_bytes == nullptr)
			throw RecognitionException("Argument 'image_bytes' cannot be null");
		if (scale_factor >= 0 && scale_factor <= 1.0)
			throw RecognitionException("Argument 'scale_factor' should be greater than 1.0 (or less than 0 to use as default value)");

		MultiClassViolaJonesController* controller = (MultiClassViolaJonesController*)_multi_class_viola_jones_controller;

		try
		{
			auto image = ImageLoader::load(image_bytes, image_bytes_length);
			auto rec_result = controller->recognize(image, classifier_ids, min_object_size, max_object_size, scale_factor, step_factor);
			return transform_results(rec_result);
		}
		catch (ImageLoadingException& il_exc)
		{
			throw RecognitionException(std::string("Image loading exception: ") + il_exc.what());
		}
		catch (LibraryException& l_exc)
		{
			throw RecognitionException(std::string("Library exception: ") + l_exc.what());
		}
	}
	RecognitionResult RecognitionController::perform_recognition(const void* image_bytes, size_t image_bytes_length, const int* classifier_ids, size_t classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
	{
		if (classifier_ids == nullptr || classifier_ids_length == 0)
			return this->perform_recognition(image_bytes, image_bytes_length, std::vector<int>(), min_object_size, max_object_size, scale_factor, step_factor);

		std::vector<int> ids(classifier_ids_length);
		for (size_t i = 0; i < classifier_ids_length; i++)
			ids[i] = classifier_ids[i];

		return this->perform_recognition(image_bytes, image_bytes_length, ids, min_object_size, max_object_size, scale_factor, step_factor);
	}
	RecognitionResult RecognitionController::perform_recognition(const void* image_bytes, size_t image_bytes_length) const
	{
		return this->perform_recognition(image_bytes, image_bytes_length, std::vector<int>(), -1, -1, -1.0, -1.0);
	}





	RecognitionResult RecognitionController::perform_recognition(const char* image_file_name, const int* classifier_ids, size_t classifier_ids_length, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
	{
		if (image_file_name == nullptr)
			throw RecognitionException("Argument 'image_file_name' cannot be null");

		if (classifier_ids == nullptr || classifier_ids_length == 0)
			return this->perform_recognition(std::string(image_file_name), std::vector<int>(), min_object_size, max_object_size, scale_factor, step_factor);

		std::vector<int> ids(classifier_ids_length);
		for (size_t i = 0; i < classifier_ids_length; i++)
			ids[i] = classifier_ids[i];

		return this->perform_recognition(std::string(image_file_name), ids, min_object_size, max_object_size, scale_factor, step_factor);
	}
	RecognitionResult RecognitionController::perform_recognition(const char* image_file_name) const
	{
		return this->perform_recognition(image_file_name, nullptr, 0, -1, -1, -1.0, -1.0);
	}
}