#pragma once
#include <assert.h>
#include <vector>
#include <algorithm>
#include "inc/classifiers/multi_class_viola_jones/ResultByClassifier.h"

class MultiClassViolaJonesRecognitionResult
{
public:
	MultiClassViolaJonesRecognitionResult()
	{
		_best_classif_id = 0;
		_best_accuracy = 0.0;
		_prepare_time_ms = 0;
		_recognition_time_ms = 0;
		_post_op_time_ms = 0;
		_overall_time_ms = 0;
	}

	MultiClassViolaJonesRecognitionResult(const std::vector<ResultByClassifier>& overall_results, int best_classif_if, double best_accuracy,
		int prepare_time_ms, int recognition_time_ms, int post_op_time_ms, int overall_time_ms)
	{
		assert(prepare_time_ms >= 0);
		assert(recognition_time_ms >= 0);
		assert(post_op_time_ms >= 0);
		assert(overall_time_ms >= 0);

		_overall_results = overall_results;
		_best_classif_id = best_classif_if;
		_best_accuracy = best_accuracy;
		_prepare_time_ms = prepare_time_ms;
		_recognition_time_ms = recognition_time_ms;
		_post_op_time_ms = post_op_time_ms;
		_overall_time_ms = overall_time_ms;
	}

	MultiClassViolaJonesRecognitionResult(std::vector<ResultByClassifier>&& overall_results, int best_classif_if, double best_accuracy,
		int prepare_time_ms, int recognition_time_ms, int post_op_time_ms, int overall_time_ms)
	{
		assert(prepare_time_ms >= 0);
		assert(recognition_time_ms >= 0);
		assert(post_op_time_ms >= 0);
		assert(overall_time_ms >= 0);

		_overall_results = std::move(overall_results);
		_best_classif_id = best_classif_if;
		_best_accuracy = best_accuracy;
		_prepare_time_ms = prepare_time_ms;
		_recognition_time_ms = recognition_time_ms;
		_post_op_time_ms = post_op_time_ms;
		_overall_time_ms = overall_time_ms;
	}

	MultiClassViolaJonesRecognitionResult(const MultiClassViolaJonesRecognitionResult& other)
	{
		this->_overall_results = other._overall_results;
		this->_best_classif_id = other._best_classif_id;
		this->_best_accuracy = other._best_accuracy;
		this->_prepare_time_ms = other._prepare_time_ms;
		this->_recognition_time_ms = other._recognition_time_ms;
		this->_post_op_time_ms = other._post_op_time_ms;
		this->_overall_time_ms = other._overall_time_ms;
	}

	MultiClassViolaJonesRecognitionResult(MultiClassViolaJonesRecognitionResult&& other)
	{
		this->_overall_results = std::move(other._overall_results);
		this->_best_classif_id = other._best_classif_id;
		this->_best_accuracy = other._best_accuracy;
		this->_prepare_time_ms = other._prepare_time_ms;
		this->_recognition_time_ms = other._recognition_time_ms;
		this->_post_op_time_ms = other._post_op_time_ms;
		this->_overall_time_ms = other._overall_time_ms;

		other._best_classif_id = 0;
		other._best_accuracy = 0;
		other._prepare_time_ms = 0;
		other._recognition_time_ms = 0;
		other._post_op_time_ms = 0;
		other._overall_time_ms = 0;
	}

	MultiClassViolaJonesRecognitionResult& operator = (const MultiClassViolaJonesRecognitionResult& other)
	{
		if (this != &other)
		{
			this->_overall_results = other._overall_results;
			this->_best_classif_id = other._best_classif_id;
			this->_best_accuracy = other._best_accuracy;
			this->_prepare_time_ms = other._prepare_time_ms;
			this->_recognition_time_ms = other._recognition_time_ms;
			this->_post_op_time_ms = other._post_op_time_ms;
			this->_overall_time_ms = other._overall_time_ms;
		}

		return *this;
	}

	MultiClassViolaJonesRecognitionResult& operator = (MultiClassViolaJonesRecognitionResult&& other)
	{
		if (this != &other)
		{
			this->_overall_results = std::move(other._overall_results);
			this->_best_classif_id = other._best_classif_id;
			this->_best_accuracy = other._best_accuracy;
			this->_prepare_time_ms = other._prepare_time_ms;
			this->_recognition_time_ms = other._recognition_time_ms;
			this->_post_op_time_ms = other._post_op_time_ms;
			this->_overall_time_ms = other._overall_time_ms;

			other._best_classif_id = 0;
			other._best_accuracy = 0;
			other._prepare_time_ms = 0;
			other._recognition_time_ms = 0;
			other._post_op_time_ms = 0;
			other._overall_time_ms = 0;
		}

		return *this;
	}

	inline const std::vector<ResultByClassifier>& get_overall_results() const
	{
		return _overall_results;
	}

	inline int get_best_classif_id() const
	{
		return _best_classif_id;
	}

	inline double get_best_accuracy() const
	{
		return _best_accuracy;
	}

	inline int get_prepare_time_ms() const
	{
		return _prepare_time_ms;
	}
		   
	inline int get_recognition_time_ms() const
	{
		return _recognition_time_ms;
	}
		   
	inline int get_post_op_time_ms() const
	{
		return _post_op_time_ms;
	}
		   
	inline int get_overall_time_ms() const
	{
		return _overall_time_ms;
	}

	void sort_overall_results_by_accuracy()
	{
		std::sort(_overall_results.begin(), _overall_results.end(), [](const ResultByClassifier& a, const ResultByClassifier& b)
		{
			if (a.get_areas_info().size() == 0 && b.get_areas_info().size() == 0)
				return false;
			if (a.get_areas_info().size() == 0)
				return false;
			if (b.get_areas_info().size() == 0)
				return true;

			return (a.get_areas_info()[0].get_accuracy() > b.get_areas_info()[0].get_accuracy());
		});
	}

private:
	std::vector<ResultByClassifier> _overall_results;
	int _best_classif_id;
	double _best_accuracy;
	int _prepare_time_ms;
	int _recognition_time_ms;
	int _post_op_time_ms;
	int _overall_time_ms;
};