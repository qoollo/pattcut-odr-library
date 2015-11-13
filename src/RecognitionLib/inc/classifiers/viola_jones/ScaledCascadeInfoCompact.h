#pragma once
#include <assert.h>
#include "inc/classifiers/cascade/CascadeClassifierCompact.h"

class ScaledCascadeInfoCompact
{
public:
	ScaledCascadeInfoCompact()
	{
		_wnd_width = 0;
		_wnd_height = 0;
		_scale_factor = 0;
		_inv_scale_factor = 0;
		_step = 0;
	}

	ScaledCascadeInfoCompact(const CascadeClassifierCompact& cascade_classifier, int wnd_width, int wnd_height, double scale_factor, double step)
	{
		assert(wnd_width > 0);
		assert(wnd_height > 0);
		assert(step >= 1.0);
		assert(scale_factor > 0.0);

		_cascade_classifier = cascade_classifier;
		_wnd_width = wnd_width;
		_wnd_height = wnd_height;
		_scale_factor = scale_factor;
		_inv_scale_factor = 1.0 / scale_factor;
		_step = step;
	}

	ScaledCascadeInfoCompact(CascadeClassifierCompact&& cascade_classifier, int wnd_width, int wnd_height, double scale_factor, double step)
	{
		assert(wnd_width > 0);
		assert(wnd_height > 0);
		assert(step >= 1.0);
		assert(scale_factor > 0.0);

		_cascade_classifier = std::move(cascade_classifier);
		_wnd_width = wnd_width;
		_wnd_height = wnd_height;
		_scale_factor = scale_factor;
		_inv_scale_factor = 1.0 / scale_factor;
		_step = step;
	}

	ScaledCascadeInfoCompact(const ScaledCascadeInfoCompact& other)
	{
		_cascade_classifier = other._cascade_classifier;
		_wnd_width = other._wnd_width;
		_wnd_height = other._wnd_height;
		_scale_factor = other._scale_factor;
		_inv_scale_factor = other._inv_scale_factor;
		_step = other._step;
	}

	ScaledCascadeInfoCompact(ScaledCascadeInfoCompact&& other)
	{
		_cascade_classifier = std::move(other._cascade_classifier);
		_wnd_width = other._wnd_width;
		_wnd_height = other._wnd_height;
		_scale_factor = other._scale_factor;
		_inv_scale_factor = other._inv_scale_factor;
		_step = other._step;
	}

	ScaledCascadeInfoCompact& operator = (const ScaledCascadeInfoCompact& other)
	{
		if (this == &other)
			return *this; 
		this->_cascade_classifier = other._cascade_classifier;
		this->_wnd_width = other._wnd_width;
		this->_wnd_height = other._wnd_height;
		this->_scale_factor = other._scale_factor;
		this->_inv_scale_factor = other._inv_scale_factor;
		this->_step = other._step;
		return *this;
	}

	ScaledCascadeInfoCompact& operator = (ScaledCascadeInfoCompact&& other)
	{
		if (this == &other)
			return *this;
		
		this->_cascade_classifier = std::move(other._cascade_classifier);
		this->_wnd_width = other._wnd_width;
		this->_wnd_height = other._wnd_height;
		this->_scale_factor = other._scale_factor;
		this->_inv_scale_factor = other._inv_scale_factor;
		this->_step = other._step;
		return *this;
	}

	inline int get_wnd_width() const
	{
		return _wnd_width;
	}

	inline int get_wnd_height() const
	{
		return _wnd_height;
	}

	inline double get_scale_factor() const
	{
		return _scale_factor;
	}

	inline double get_inv_scale_factor() const
	{
		return _inv_scale_factor;
	}

	inline double get_step() const
	{
		return _step;
	}

	inline const CascadeClassifierCompact& get_cascade_classifier() const
	{
		return _cascade_classifier;
	}

private:
	CascadeClassifierCompact _cascade_classifier;
	int _wnd_width;
	int _wnd_height;
	double _scale_factor;
	double _inv_scale_factor;
	double _step;
};