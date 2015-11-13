#pragma once
#include <assert.h>
#include <vector>
#include <algorithm>
#include "inc/classifiers/multi_class_viola_jones/CapturedAreaInfo.h"

class ResultByClassifier
{
public:
	ResultByClassifier()
	{
		_class_id = 0;
		_rec_time_ms = 0;
		_wnd_count = 0;
		_mean_weak_count = 0;
		_mean_depth = 0;
		_wnd_after_prefilter = 0;
	}

	ResultByClassifier(const std::vector<CapturedAreaInfo>& areas_info, int class_id, int time_ms, int wnd_count, int wnd_after_prefilter,
		double mean_weak_count, double mean_depth)
	{
		assert(time_ms >= 0);
		assert(wnd_count >= 0);
		assert(wnd_after_prefilter >= 0);
		assert(mean_weak_count >= 0);
		assert(mean_depth >= 0);

		_areas_info = areas_info;
		_class_id = class_id;
		_rec_time_ms = time_ms;
		_wnd_count = wnd_count;
		_mean_weak_count = mean_weak_count;
		_mean_depth = mean_depth;
		_wnd_after_prefilter = wnd_after_prefilter;
	}

	ResultByClassifier(std::vector<CapturedAreaInfo>&& areas_info, int class_id, int time_ms, int wnd_count, int wnd_after_prefilter,
		double mean_weak_count, double mean_depth)
	{
		assert(time_ms >= 0);
		assert(wnd_count >= 0);
		assert(wnd_after_prefilter >= 0);
		assert(mean_weak_count >= 0);
		assert(mean_depth >= 0);

		_areas_info = std::move(areas_info);
		_class_id = class_id;
		_rec_time_ms = time_ms;
		_wnd_count = wnd_count;
		_mean_weak_count = mean_weak_count;
		_mean_depth = mean_depth;
		_wnd_after_prefilter = wnd_after_prefilter;
	}

	ResultByClassifier(const ResultByClassifier& other)
	{
		this->_areas_info = other._areas_info;
		this->_class_id = other._class_id;
		this->_rec_time_ms = other._rec_time_ms;
		this->_wnd_count = other._wnd_count;
		this->_mean_weak_count = other._mean_weak_count;
		this->_mean_depth = other._mean_depth;
		this->_wnd_after_prefilter = other._wnd_after_prefilter;
	}

	ResultByClassifier(ResultByClassifier&& other)
	{
		this->_areas_info = std::move(other._areas_info);
		this->_class_id = other._class_id;
		this->_rec_time_ms = other._rec_time_ms;
		this->_wnd_count = other._wnd_count;
		this->_mean_weak_count = other._mean_weak_count;
		this->_mean_depth = other._mean_depth;
		this->_wnd_after_prefilter = other._wnd_after_prefilter;

		other._class_id = 0;
		other._rec_time_ms = 0;
		other._wnd_count = 0;
		other._mean_weak_count = 0;
		other._mean_depth = 0;
		other._wnd_after_prefilter = 0;
	}

	ResultByClassifier& operator=(const ResultByClassifier& other)
	{
		if (this != &other)
		{
			this->_areas_info = other._areas_info;
			this->_class_id = other._class_id;
			this->_rec_time_ms = other._rec_time_ms;
			this->_wnd_count = other._wnd_count;
			this->_mean_weak_count = other._mean_weak_count;
			this->_mean_depth = other._mean_depth;
			this->_wnd_after_prefilter = other._wnd_after_prefilter;
		}

		return *this;
	}

	ResultByClassifier& operator=(ResultByClassifier&& other)
	{
		if (this != &other)
		{
			this->_areas_info = std::move(other._areas_info);
			this->_class_id = other._class_id;
			this->_rec_time_ms = other._rec_time_ms;
			this->_wnd_count = other._wnd_count;
			this->_mean_weak_count = other._mean_weak_count;
			this->_mean_depth = other._mean_depth;
			this->_wnd_after_prefilter = other._wnd_after_prefilter;

			other._class_id = 0;
			other._rec_time_ms = 0;
			other._wnd_count = 0;
			other._mean_weak_count = 0;
			other._mean_depth = 0;
			other._wnd_after_prefilter = 0;
		}

		return *this;
	}

	inline const std::vector<CapturedAreaInfo>& get_areas_info() const
	{
		return _areas_info;
	}

	inline int get_class_id() const
	{
		return _class_id;
	}

	inline int get_rec_time_ms() const
	{
		return _rec_time_ms;
	}

	inline int get_wnd_count() const
	{
		return _wnd_count;
	}

	inline double get_mean_weak_count() const
	{
		return _mean_weak_count;
	}

	inline double get_mean_depth() const
	{
		return _mean_depth;
	}

	inline int get_wnd_after_prefilter() const
	{
		return _wnd_after_prefilter;
	}

	void sort_areas_by_acuracy()
	{
		std::sort(_areas_info.begin(), _areas_info.end(), [](const CapturedAreaInfo& a, const CapturedAreaInfo& b)
		{
			return (a.get_accuracy() > b.get_accuracy());
		});
	}

private:
	std::vector<CapturedAreaInfo> _areas_info;
	int _class_id;
	int _rec_time_ms;
	int _wnd_count;
	double _mean_weak_count;
	double _mean_depth;
	int _wnd_after_prefilter;
};