#include "inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesController.h"
#include <math.h>
#include <chrono>
#include <algorithm>
#include "inc/images/IntegralImage.h"

const double MultiClassViolaJonesController::scale_to = 0.2;

MultiClassViolaJonesController::MultiClassViolaJonesController()
{

}

MultiClassViolaJonesController::MultiClassViolaJonesController(const MultiClassViolaJonesController& other)
{
	this->_recognizers = other._recognizers;
	this->_res_filter = other._res_filter;
}

MultiClassViolaJonesController::MultiClassViolaJonesController(MultiClassViolaJonesController&& other)
{
	this->_recognizers = std::move(other._recognizers);
	this->_res_filter = other._res_filter;
}

MultiClassViolaJonesController& MultiClassViolaJonesController::operator=(const MultiClassViolaJonesController& other)
{
	if (this != &other)
	{
		this->_recognizers = other._recognizers;
		this->_res_filter = other._res_filter;
	}

	return *this;
}

MultiClassViolaJonesController& MultiClassViolaJonesController::operator=(MultiClassViolaJonesController&& other)
{
	if (this != &other)
	{
		this->_recognizers = std::move(other._recognizers);
		this->_res_filter = other._res_filter;
	}

	return *this;
}

const ViolaJonesClassifier& MultiClassViolaJonesController::get_recognizer_by_id(int id) const
{
	for (auto& rec : _recognizers)
	{
		if (rec.get_classif_id() == id)
			return rec.get_controller();
	}

	throw LibraryException(std::string("get_recognizer_by_id: recognizer not found. id = ") + std::to_string(id));
}

bool MultiClassViolaJonesController::has_recognizer(int id)
{
	for (auto& rec : _recognizers)
	{
		if (rec.get_classif_id() == id)
			return true;
	}

	return false;
}

LogisticAccuracyEstimator MultiClassViolaJonesController::create_accuracy_estimator(const ViolaJonesClassifier& controller)
{
	double comb_max = evaluate_max_comb_value(controller);
	double window_norm_coef = LogisticAccuracyEstimator::default_window_norm_coef;
	double threshold = LogisticAccuracyEstimator::default_threshold;

	return LogisticAccuracyEstimator(window_norm_coef, comb_max, threshold);
}

void MultiClassViolaJonesController::add_recognizer(int id, const ViolaJonesClassifier& controller)
{
	MultiClassViolaJonesRecognitionDataInfo elem = MultiClassViolaJonesRecognitionDataInfo(id, controller, create_accuracy_estimator(controller));

	_recognizers.erase(std::remove_if(_recognizers.begin(), _recognizers.end(), [&id](const MultiClassViolaJonesRecognitionDataInfo& a)
	{
		return a.get_classif_id() == id;
	}), _recognizers.end());


	_recognizers.push_back(std::move(elem));
}

void MultiClassViolaJonesController::add_recognizer(int id, ViolaJonesClassifier&& controller)
{
	auto estimator = create_accuracy_estimator(controller);
	MultiClassViolaJonesRecognitionDataInfo elem = MultiClassViolaJonesRecognitionDataInfo(id, std::move(controller), estimator);

	_recognizers.erase(std::remove_if(_recognizers.begin(), _recognizers.end(), [&id](const MultiClassViolaJonesRecognitionDataInfo& a)
	{
		return a.get_classif_id() == id;
	}), _recognizers.end());

	_recognizers.push_back(std::move(elem));
}

double MultiClassViolaJonesController::evaluate_max_comb_value(const ViolaJonesClassifier& controller)
{
	const CascadeClassifier& casc = controller.get_cascade_classifier();

	double total_aggr = 0.0;
	int weak_norm = 0;
	
	for (unsigned int i = 0; i < casc.get_classifier_count(); i++)
	{
		int cur_weak_cnt = 0;
		double cur_aggr = 0.0;
		
		const BoostedClassifier& boosted_cls = casc.get_classifier_at(i);

		for (unsigned int j = 0; j < boosted_cls.get_classifier_count(); j++)
		{
			const StumpClassifier& stump_cls = boosted_cls.get_classifier_at(j);

			if (stump_cls.get_result_on_greater().tag() == boosted_cls.get_correct_tag())
				cur_aggr += abs(stump_cls.get_result_on_greater().confidence());
			else
				cur_aggr += abs(stump_cls.get_result_on_less().confidence());
			cur_weak_cnt++;
		}

		cur_aggr -= boosted_cls.get_threshold();
		total_aggr += cur_aggr * cur_weak_cnt;
		weak_norm += cur_weak_cnt;
	}

	return total_aggr / weak_norm;
}



MultiClassViolaJonesRecognitionResult MultiClassViolaJonesController::recognize(const Image& image, const std::vector<int>& classifier_ids, int min_object_size, int max_object_size, double scale_factor, double step_factor) const
{
	auto total_start = std::chrono::system_clock::now();
	std::vector<ResultByClassifier> tot_res;
	tot_res.reserve(_recognizers.size());

	auto prepare_start = std::chrono::system_clock::now();

	IntegralImage int_img(&image);

	auto prepare_period = std::chrono::system_clock::now() - prepare_start;

	auto recognition_start = std::chrono::system_clock::now();

	for (unsigned int i = 0; i < _recognizers.size(); i++)
	{
		if (classifier_ids.size() > 0 && std::find(classifier_ids.begin(), classifier_ids.end(), _recognizers[i].get_classif_id()) == classifier_ids.end())
			continue;

		auto loc_rec_start = std::chrono::system_clock::now();

		auto res_non_filt = _recognizers[i].get_controller().apply_with_statistic(int_img, min_object_size, max_object_size, scale_factor, step_factor);

		auto loc_rec_period = std::chrono::system_clock::now() - loc_rec_start;

		auto filtered_res = _res_filter.filter(res_non_filt);

		std::vector<CapturedAreaInfo> loc_area_infos(filtered_res.areas().size());

		for (unsigned int area_num = 0; area_num < filtered_res.areas().size(); area_num++)
		{
			auto estim = _recognizers[i].get_accuracy_estimator().estimate(filtered_res.areas()[area_num]);
			loc_area_infos[area_num] = CapturedAreaInfo(_recognizers[i].get_classif_id(), filtered_res.areas()[area_num], estim);
		}

		tot_res.push_back(ResultByClassifier(loc_area_infos, _recognizers[i].get_classif_id(), (int)std::chrono::duration_cast<std::chrono::milliseconds>(loc_rec_period).count(),
			filtered_res.get_wnd_count(), filtered_res.get_wnd_after_prefilter(), filtered_res.get_mean_weak_count(), filtered_res.get_mean_depth()));
	}

	auto recognition_period = std::chrono::system_clock::now() - recognition_start;

	auto post_start = std::chrono::system_clock::now();

	for (unsigned int i = 0; i < tot_res.size(); i++)
	{
		tot_res[i].sort_areas_by_acuracy();
	}

	std::sort(tot_res.begin(), tot_res.end(), [](const ResultByClassifier& a, const ResultByClassifier& b)
	{
		if (a.get_areas_info().size() == 0 && b.get_areas_info().size() == 0)
			return false;
		if (a.get_areas_info().size() == 0)
			return false;
		if (b.get_areas_info().size() == 0)
			return true;

		return (a.get_areas_info()[0].get_accuracy() > b.get_areas_info()[0].get_accuracy());
	});

	int best_cls_id = -1;
	double best_acc = 0.0;
	if (tot_res.size() > 0 && tot_res[0].get_areas_info().size() > 0 && tot_res[0].get_areas_info()[0].get_is_good())
	{
		best_cls_id = tot_res[0].get_areas_info()[0].get_class_id();
		best_acc = tot_res[0].get_areas_info()[0].get_accuracy();
	}

	auto post_period = std::chrono::system_clock::now() - post_start;

	return MultiClassViolaJonesRecognitionResult(std::move(tot_res), best_cls_id, best_acc,
		(int)std::chrono::duration_cast<std::chrono::milliseconds>(prepare_period).count(),
		(int)std::chrono::duration_cast<std::chrono::milliseconds>(recognition_period).count(),
		(int)std::chrono::duration_cast<std::chrono::milliseconds>(post_period).count(),
		(int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - total_start).count());
}