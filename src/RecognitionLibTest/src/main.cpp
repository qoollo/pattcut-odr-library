#include <iostream>
#include <fstream>
#include "RecognitionLib/inc/export/test_export.h"
#include "RecognitionLib/inc/export/pattcut/PattCutCPP.h"
#include "RecognitionLib/inc/export/pattcut/PattCutC.h"
#include <memory>
#include <vector>
#include <list>



void run_cpp_recongition(const std::string& classifier_path, const std::string& image_path)
{
	PattCutLib::RecognitionController controller;

	try
	{
		auto classifier_id = controller.load_classifier_from_file(classifier_path);
		std::cout << "Classifier loaded. Classifier id = " << classifier_id.id() << std::endl;

		int time_cum = 0;
		for (int i = 0; i < 10; i++)
		{
			auto res = controller.perform_recognition(image_path);
			std::cout << "total time: " << res.total_time_ms() << std::endl;
			if (res.best_classifier_id() > 0)
			{
				std::cout << "recognized!" << std::endl;
			}
			for (int i = 0; i < res.result_per_classifier().size(); i++)
			{
				auto cur_area = res.result_per_classifier()[0].areas()[i];
				std::cout << "veracity: " << cur_area.veracity() <<
					", wnd_count: " << cur_area.merged_window_count() <<
					", area: [" << cur_area.rect_x() << ", " << cur_area.rect_y() << "; " << cur_area.rect_width() << ", " << cur_area.rect_height() << "]" << std::endl;
			}
			time_cum += res.total_time_ms();
		}
		std::cout << "accumulated time: " << time_cum / 10.0 << std::endl;
	}
	catch (PattCutLib::RecognitionException& exc)
	{
		std::cout << "Error: " << exc.what() << std::endl;
	}
}

void run_c_recongition(const std::string& classifier_path, const std::string& image_path)
{
	char* error_message;

	auto ctrl = pattcut_init_controller();
	auto cls_data_id = pattcut_load_classifier_from_file(ctrl, classifier_path.c_str(), &error_message);
	if (cls_data_id.id < 0)
		std::cout << "Clssifier not loaded. Reason: " << error_message << std::endl;

	pattcut_free_error_message(&error_message);
	int time_cum = 0;
	for (int i = 0; i < 10; i++)
	{
		auto res = pattcut_perform_recognition_on_image_file(ctrl, image_path.c_str(), nullptr, 0, 24, -1, 1.1, 1.4, &error_message);
		if (res == nullptr)
		{
			std::cout << "Recognition error: " << error_message << std::endl;
		}
		else
		{
			std::cout << "total time: " << res->total_time_ms << std::endl;
			if (res->best_classifier_id > 0)
			{
				std::cout << "recognized!" << std::endl;
			}
			for (int i = 0; i < res->result_per_classifier[0].areas_count; i++)
			{
				auto cur_area = res->result_per_classifier[0].areas[i];
				std::cout << "veracity: " << cur_area.veracity <<
					", wnd_count: " << cur_area.merged_window_count <<
					", area: [" << cur_area.rect_x << ", " << cur_area.rect_y << "; " << cur_area.rect_width << ", " << cur_area.rect_height << "]" << std::endl;
			}
			time_cum += res->total_time_ms;
		}
		pattcut_free_error_message(&error_message);
		pattcut_free_recognition_results(&res);
	}
	std::cout << "accumulated time: " << time_cum / 10.0 << std::endl;
	pattcut_destroy_controller(&ctrl);
}

int main()
{
	std::string classifier_path = "haarcascade_frontalface_default.xml";
	std::string image_path = "face_large.jpg";

	run_cpp_recongition(classifier_path, image_path);
	getchar();

	run_c_recongition(classifier_path, image_path);
	getchar();

	return 0;
}
