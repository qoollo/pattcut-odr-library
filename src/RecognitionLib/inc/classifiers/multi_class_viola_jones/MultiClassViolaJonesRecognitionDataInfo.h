#pragma once
#include "inc/classifiers/viola_jones/ViolaJonesClassifier.h"
#include "inc/accuracy_estimation/LogisticAccuracyEstimator.h"

class MultiClassViolaJonesRecognitionDataInfo
{
public:
	MultiClassViolaJonesRecognitionDataInfo()
	{
		_classif_id = 0;
	}

	MultiClassViolaJonesRecognitionDataInfo(int classif_id, const ViolaJonesClassifier& controller, const LogisticAccuracyEstimator& accuracy_estimator)
	{
		_classif_id = classif_id;
		_controller = controller;
		_accuracy_estimator = accuracy_estimator;
	}

	MultiClassViolaJonesRecognitionDataInfo(int classif_id, ViolaJonesClassifier&& controller, const LogisticAccuracyEstimator& accuracy_estimator)
	{
		_classif_id = classif_id;
		_controller = std::move(controller);
		_accuracy_estimator = accuracy_estimator;
	}

	MultiClassViolaJonesRecognitionDataInfo(const MultiClassViolaJonesRecognitionDataInfo& other)
	{
		this->_classif_id = other._classif_id;
		this->_controller = other._controller;
		this->_accuracy_estimator = other._accuracy_estimator;
	}

	MultiClassViolaJonesRecognitionDataInfo(MultiClassViolaJonesRecognitionDataInfo&& other)
	{
		this->_classif_id = other._classif_id;
		this->_controller = std::move(other._controller);
		this->_accuracy_estimator = std::move(other._accuracy_estimator);

		other._classif_id = 0;
	}

	MultiClassViolaJonesRecognitionDataInfo& operator = (const MultiClassViolaJonesRecognitionDataInfo& other)
	{
		if (this != &other)
		{
			this->_classif_id = other._classif_id;
			this->_controller = other._controller;
			this->_accuracy_estimator = other._accuracy_estimator;
		}

		return *this;
	}

	MultiClassViolaJonesRecognitionDataInfo& operator = (MultiClassViolaJonesRecognitionDataInfo&& other)
	{
		if (this != &other)
		{
			this->_classif_id = other._classif_id;
			this->_controller = std::move(other._controller);
			this->_accuracy_estimator = std::move(other._accuracy_estimator);

			other._classif_id = 0;
		}

		return *this;
	}

	inline int get_classif_id() const
	{
		return _classif_id;
	}

	inline const ViolaJonesClassifier& get_controller() const
	{
		return _controller;
	}

	inline const LogisticAccuracyEstimator& get_accuracy_estimator() const
	{
		return _accuracy_estimator;
	}

private:
	int _classif_id;
	ViolaJonesClassifier _controller;
	LogisticAccuracyEstimator _accuracy_estimator;
};