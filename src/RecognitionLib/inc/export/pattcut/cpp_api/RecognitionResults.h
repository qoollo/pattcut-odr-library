#pragma once
#include <vector>
#include <utility>
#include "pattcut/export_helper.h"

namespace PattCutLib
{
	/** Area of the image, containing detected object */
	class CapturedArea
	{
	public:
		/** CapturedArea constructor */
		CapturedArea() 
		{
			_rect_x = 0;
			_rect_y = 0;
			_rect_width = 0;
			_rect_height = 0;
			_veracity = 0;
			_is_good = false;
		}
		/**
		 * CapturedArea constructor
		 * @rect_x - X coordinate of the area rectangle
		 * @rect_y - Y coordinate of the area rectangle
		 * @rect_width - Width of the area rectangle
		 * @rect_height - Height of the area rectangle
		 * @veracity - Veracity of detection for the current area
		 * @is_good - Is this area passed a minimum veracity threshold (usually threshold = 0)
		 * @merged_window_count - Window count that formed the current area
		 * @combined_strong_cls_output - Accuracy metric calculated from strong classifiers output
		 */
		CapturedArea(int rect_x, int rect_y, int rect_width, int rect_height, double veracity, bool is_good, int merged_window_count, double combined_strong_cls_output)
		{
			_rect_x = rect_x;
			_rect_y = rect_y;
			_rect_width = rect_width;
			_rect_height = rect_height;
			_veracity = veracity;
			_is_good = is_good;
			_merged_window_count = merged_window_count;
			_combined_strong_cls_output = combined_strong_cls_output;
		}

		/** X coordinate of the area rectangle */
		int rect_x() const { return _rect_x; }
		/** Y coordinate of the area rectangle */
		int rect_y() const { return _rect_y; }
		/** Width of the area rectangle */
		int rect_width() const { return _rect_width; }
		/** Height of the area rectangle */
		int rect_height() const { return _rect_height; }

		/** Veracity of detection for the current area */
		double veracity() const { return _veracity; }
		/** Is this area passed a minimum veracity threshold (usually threshold = 0) */
		bool is_good() const { return _is_good; }

		/** Window count that formed the current area */
		int merged_window_count() const { return _merged_window_count; }
		/** Accuracy metric calculated from strong classifiers output */
		double combined_strong_cls_output() const { return _combined_strong_cls_output; }

	private:
		int _rect_x;
		int _rect_y;
		int _rect_width;
		int _rect_height;

		double _veracity;
		bool _is_good;

		int _merged_window_count;
		double _combined_strong_cls_output;
	};


	/** Recognition result for a separate classifier */
	class DLL_EXPORT ResultPerClassifier
	{
	public:
		/** ResultPerClassifier constructor */
		ResultPerClassifier()
		{
			_classifier_id = 0;
			_recognition_time_ms = 0;
			_window_count = 0;
			_window_count_after_prefilter = 0;
			_mean_depth = 0;
			_mean_weak_cls_count = 0;
			_areas = new std::vector<CapturedArea>();
		}
		/** ResultPerClassifier copy constructor */
		ResultPerClassifier(const ResultPerClassifier& source)
		{
			_classifier_id = source._classifier_id;
			_recognition_time_ms = source._recognition_time_ms;
			_window_count = source._window_count;
			_window_count_after_prefilter = source._window_count_after_prefilter;
			_mean_depth = source._mean_depth;
			_mean_weak_cls_count = source._mean_weak_cls_count;
			_areas = new std::vector<CapturedArea>(*source._areas);
		}
		/** ResultPerClassifier move constructor */
		ResultPerClassifier(ResultPerClassifier&& source)
		{
			_classifier_id = source._classifier_id;
			_recognition_time_ms = source._recognition_time_ms;
			_window_count = source._window_count;
			_window_count_after_prefilter = source._window_count_after_prefilter;
			_mean_depth = source._mean_depth;
			_mean_weak_cls_count = source._mean_weak_cls_count;
			_areas = new std::vector<CapturedArea>(std::move(*source._areas));
		}
		/** ResultPerClassifier copy operator */
		ResultPerClassifier& operator=(const ResultPerClassifier& source)
		{
			if (this != &source)
			{
				_classifier_id = source._classifier_id;
				_recognition_time_ms = source._recognition_time_ms;
				_window_count = source._window_count;
				_window_count_after_prefilter = source._window_count_after_prefilter;
				_mean_depth = source._mean_depth;
				_mean_weak_cls_count = source._mean_weak_cls_count;
				_areas = new std::vector<CapturedArea>(*source._areas);
			}
			return *this;
		}
		/** ResultPerClassifier move operator */
		ResultPerClassifier& operator=(ResultPerClassifier&& source)
		{
			if (this != &source)
			{
				_classifier_id = source._classifier_id;
				_recognition_time_ms = source._recognition_time_ms;
				_window_count = source._window_count;
				_window_count_after_prefilter = source._window_count_after_prefilter;
				_mean_depth = source._mean_depth;
				_mean_weak_cls_count = source._mean_weak_cls_count;
				_areas = new std::vector<CapturedArea>(std::move(*source._areas));
			}
			return *this;
		}

		/**
		 * ResultPerClassifier construcotr
		 * @classifier_id - Identifier of the classifier that was generated by 'load_classifier' method
		 * @recognition_time_ms - Recognition time for the current classifier
		 * @window_count - Total analized window count
		 * @window_count_after_prefilter - Total window count that passed the prefilter classifier
		 * @mean_depth - Average number of estimated strong classifiers
		 * @mean_weak_cls_count - Average number of estimated weak classifiers
		 * @areas - Vector with all detected areas by this classifier
		 */
		ResultPerClassifier(int classifier_id, int recognition_time_ms, int window_count, int window_count_after_prefilter, double mean_depth, double mean_weak_cls_count, const std::vector<CapturedArea>& areas)
		{
			_classifier_id = classifier_id;
			_recognition_time_ms = recognition_time_ms;
			_window_count = window_count;
			_window_count_after_prefilter = window_count_after_prefilter;
			_mean_depth = mean_depth;
			_mean_weak_cls_count = mean_weak_cls_count;
			_areas = new std::vector<CapturedArea>(areas);
		}
		/**
		 * ResultPerClassifier construcotr
		 * @classifier_id - Identifier of the classifier that was generated by 'load_classifier' method
		 * @recognition_time_ms - Recognition time for the current classifier
		 * @window_count - Total analized window count
		 * @window_count_after_prefilter - Total window count that passed the prefilter classifier
		 * @mean_depth - Average number of estimated strong classifiers
		 * @mean_weak_cls_count - Average number of estimated weak classifiers
		 * @areas - Vector with all detected areas by this classifier
		 */
		ResultPerClassifier(int classifier_id, int recognition_time_ms, int window_count, int window_count_after_prefilter, double mean_depth, double mean_weak_cls_count, std::vector<CapturedArea>&& areas)
		{
			_classifier_id = classifier_id;
			_recognition_time_ms = recognition_time_ms;
			_window_count = window_count;
			_window_count_after_prefilter = window_count_after_prefilter;
			_mean_depth = mean_depth;
			_mean_weak_cls_count = mean_weak_cls_count;
			_areas = new std::vector<CapturedArea>(std::move(areas));
		}

		/** ResultPerClassifier destructor */
		~ResultPerClassifier()
		{
			if (_areas != nullptr)
			{
				delete _areas;
				_areas = nullptr;
			}
		}

		/** Identifier of the classifier that was generated by 'load_classifier' method */
		int classifier_id() const { return _classifier_id; }

		/** Recognition time for the current classifier */
		int recognition_time_ms() const { return _recognition_time_ms; }

		/** Total analized window count */
		int window_count() const { return _window_count; }
		/** Total window count that passed the prefilter classifier */
		int window_count_after_prefilter() const { return _window_count_after_prefilter; }

		/** Average number of estimated strong classifiers */
		double mean_depth() const { return _mean_depth; }
		/** Average number of estimated weak classifiers */
		double mean_weak_cls_count() const { return _mean_weak_cls_count; }

		/** Vector with all detected areas by this classifier */
		const std::vector<CapturedArea>& areas() const { return *_areas; }
		/** Returns information about particular area */
		const CapturedArea& areas(int index) const { return _areas->at(index); }

	private:
		int _classifier_id;
		int _recognition_time_ms;
		int _window_count;
		int _window_count_after_prefilter;
		double _mean_depth;
		double _mean_weak_cls_count;
		std::vector<CapturedArea>* _areas;
	};


	/** Recognition results for all used object classifiers */
	class DLL_EXPORT RecognitionResult
	{
	public:
		/** RecognitionResult constructor */
		RecognitionResult() 
		{
			_best_classifier_id = 0;
			_total_time_ms = 0;
			_result_per_classifier = new std::vector<ResultPerClassifier>();
		}
		/** RecognitionResult copy constructor */
		RecognitionResult(const RecognitionResult& source)
		{
			_best_classifier_id = source._best_classifier_id;
			_total_time_ms = source._total_time_ms;
			_result_per_classifier = new std::vector<ResultPerClassifier>(*source._result_per_classifier);
		}
		/** RecognitionResult move constructor */
		RecognitionResult(RecognitionResult&& source)
		{
			_best_classifier_id = source._best_classifier_id;
			_total_time_ms = source._total_time_ms;
			_result_per_classifier = new std::vector<ResultPerClassifier>(std::move(*source._result_per_classifier));
		}
		/** RecognitionResult copy operator */
		RecognitionResult& operator=(const RecognitionResult& source) 
		{ 
			if (this != &source)
			{
				_best_classifier_id = source._best_classifier_id;
				_total_time_ms = source._total_time_ms;
				_result_per_classifier = new std::vector<ResultPerClassifier>(*source._result_per_classifier);
			}
			return *this;
		}
		/** RecognitionResult move operator */
		RecognitionResult& operator=(RecognitionResult&& source) 
		{ 
			if (this != &source)
			{
				_best_classifier_id = source._best_classifier_id;
				_total_time_ms = source._total_time_ms;
				_result_per_classifier = new std::vector<ResultPerClassifier>(std::move(*source._result_per_classifier));
			}
			return *this;
		}

		/** 
		 * RecognitionResult constructor 
		 * @best_classifier_id - Identifier of the best classifier (classifier that detected area with maximal veracity)
		 * @total_time_ms - Total recognition time (without image loading time)
		 * @result_per_classifier - Vector with results from every used object classifier
		 */
		RecognitionResult(int best_classifier_id, int total_time_ms, const std::vector<ResultPerClassifier>& per_classifier)
		{
			_best_classifier_id = best_classifier_id;
			_total_time_ms = total_time_ms;
			_result_per_classifier = new std::vector<ResultPerClassifier>(per_classifier);
		}
		/**
		 * RecognitionResult constructor
		 * @best_classifier_id - Identifier of the best classifier (classifier that detected area with maximal veracity)
		 * @total_time_ms - Total recognition time (without image loading time)
		 * @result_per_classifier - Vector with results from every used object classifier
		 */
		RecognitionResult(int best_classifier_id, int total_time_ms, std::vector<ResultPerClassifier>&& per_classifier)
		{
			_best_classifier_id = best_classifier_id;
			_total_time_ms = total_time_ms;
			_result_per_classifier = new std::vector<ResultPerClassifier>(std::move(per_classifier));
		}

		/** RecognitionResult destructor */
		~RecognitionResult()
		{
			if (_result_per_classifier != nullptr)
			{
				delete _result_per_classifier;
				_result_per_classifier = nullptr;
			}
		}

		/** Identifier of the best classifier (classifier that detected area with maximal veracity) */
		int best_classifier_id() const { return _best_classifier_id; }
		/** Total recognition time (without image loading time) */
		int total_time_ms() const { return _total_time_ms; }
		/** Vector with results from every used object classifier */
		const std::vector<ResultPerClassifier>& result_per_classifier() const { return *_result_per_classifier; }
		/** Returns result for particular classifier */
		const ResultPerClassifier& result_per_classifier(int index) const { return _result_per_classifier->at(index); }

	private:
		int _total_time_ms;
		int _best_classifier_id;
		std::vector<ResultPerClassifier>* _result_per_classifier;
	};
}