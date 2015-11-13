#pragma once
#include <assert.h>
#include <vector>
#include "inc/post_filtration/FilteredAreaWithExtInfo.h"

class ViolaJonesFilteredOutputWithStatistic
{
public:
	ViolaJonesFilteredOutputWithStatistic()
	{
		_wnd_count = 0;
		_mean_weak_count = 0;
		_mean_depth = 0;
		_wnd_after_prefilter = 0;
	}

	ViolaJonesFilteredOutputWithStatistic(const std::vector<FilteredAreaWithExtInfo>& areas, int wnd_count, double mean_weak_count, double mean_depth, int wnd_after_prefilter)
	{
		assert(wnd_count >= 0);
		assert(mean_depth >= 0);
		assert(mean_weak_count >= 0);
		assert(wnd_after_prefilter >= 0);

		_areas = areas;
		_wnd_count = wnd_count;
		_mean_weak_count = mean_weak_count;
		_mean_depth = mean_depth;
		_wnd_after_prefilter = wnd_after_prefilter;
	}

	ViolaJonesFilteredOutputWithStatistic(std::vector<FilteredAreaWithExtInfo>&& areas, int wnd_count, double mean_weak_count, double mean_depth, int wnd_after_prefilter)
	{
		assert(wnd_count >= 0);
		assert(mean_depth >= 0);
		assert(mean_weak_count >= 0);
		assert(wnd_after_prefilter >= 0);

		_areas = areas;
		_wnd_count = wnd_count;
		_mean_weak_count = mean_weak_count;
		_mean_depth = mean_depth;
		_wnd_after_prefilter = wnd_after_prefilter;
	}

	ViolaJonesFilteredOutputWithStatistic(const ViolaJonesFilteredOutputWithStatistic& other)
	{
		this->_areas = other._areas;
		this->_wnd_count = other._wnd_count;
		this->_mean_weak_count = other._mean_weak_count;
		this->_mean_depth = other._mean_depth;
		this->_wnd_after_prefilter = other._wnd_after_prefilter;
	}

	ViolaJonesFilteredOutputWithStatistic(ViolaJonesFilteredOutputWithStatistic&& other)
	{
		this->_areas = std::move(other._areas);
		this->_wnd_count = other._wnd_count;
		this->_mean_weak_count = other._mean_weak_count;
		this->_mean_depth = other._mean_depth;
		this->_wnd_after_prefilter = other._wnd_after_prefilter;

		other._wnd_count = 0;
		other._mean_weak_count = 0;
		other._mean_depth = 0;
		other._wnd_after_prefilter = 0;
	}

	inline const std::vector<FilteredAreaWithExtInfo>& areas() const
	{
		return _areas;
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

	ViolaJonesFilteredOutputWithStatistic& operator = (const ViolaJonesFilteredOutputWithStatistic& other)
	{
		if (this != &other)
			return *this;

		this->_areas = other._areas;
		this->_wnd_count = other._wnd_count;
		this->_mean_weak_count = other._mean_weak_count;
		this->_mean_depth = other._mean_depth;
		this->_wnd_after_prefilter = other._wnd_after_prefilter;

		return *this;
	}

	ViolaJonesFilteredOutputWithStatistic& operator = (ViolaJonesFilteredOutputWithStatistic&& other)
	{
		if (this != &other)
			return *this;

		this->_areas = std::move(other._areas);
		this->_wnd_count = other._wnd_count;
		this->_mean_weak_count = other._mean_weak_count;
		this->_mean_depth = other._mean_depth;
		this->_wnd_after_prefilter = other._wnd_after_prefilter;
		
		other._wnd_count = 0;
		other._mean_weak_count = 0;
		other._mean_depth = 0;
		other._wnd_after_prefilter = 0;
		
		return *this;
	}

private:
	std::vector<FilteredAreaWithExtInfo> _areas;
	int _wnd_count;
	double _mean_weak_count;
	double _mean_depth;
	int _wnd_after_prefilter;
};