#pragma once
#include <vector>
#include <string>
#include "inc/images/Image.h"
#include "inc/classifiers/viola_jones/ViolaJonesClassifier.h"
#include "inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesRecognitionResult.h"
#include "inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesRecognitionDataInfo.h"
#include "inc/post_filtration/StandardResultFiltration.h"


class MultiClassViolaJonesController
{
public:
	MultiClassViolaJonesController();

	MultiClassViolaJonesController(const MultiClassViolaJonesController& other);
	MultiClassViolaJonesController(MultiClassViolaJonesController&& other);

	MultiClassViolaJonesController& operator = (const MultiClassViolaJonesController& other);
	MultiClassViolaJonesController& operator = (MultiClassViolaJonesController&& other);

	const ViolaJonesClassifier& get_recognizer_by_id(int id) const;

	bool has_recognizer(int id);

	void add_recognizer(int id, const ViolaJonesClassifier& controller);
	void add_recognizer(int id, ViolaJonesClassifier&& controller);

	MultiClassViolaJonesRecognitionResult recognize(const Image& image, const std::vector<int>& classifier_ids, int min_object_size, int max_object_size, double scale_factor, double step_factor) const;
	MultiClassViolaJonesRecognitionResult recognize(const Image& image, const std::vector<int>& classifier_ids) const
	{
		return recognize(image, classifier_ids, -1, -1, -1, -1);
	}
	MultiClassViolaJonesRecognitionResult recognize(const Image& image) const
	{
		return recognize(image, std::vector<int>(), -1, -1, -1, -1);
	}

	static const double scale_to;

private:
	double evaluate_max_comb_value(const ViolaJonesClassifier& controller);
	LogisticAccuracyEstimator create_accuracy_estimator(const ViolaJonesClassifier& controller);

	std::vector<MultiClassViolaJonesRecognitionDataInfo> _recognizers;
	StandardResultFiltration _res_filter;
};