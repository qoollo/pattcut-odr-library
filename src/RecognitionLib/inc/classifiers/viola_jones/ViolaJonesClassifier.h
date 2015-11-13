#pragma once
#include <assert.h>
#include <vector>
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/cascade/CascadeClassifier.h"
#include "inc/classifiers/simple/StatMomentsClassifier.h"
#include "inc/classifiers/viola_jones/ScaledCascadeInfo.h"
#include "inc/classifiers/viola_jones/ScaledCascadeInfoCompact.h"
#include "inc/classifiers/viola_jones/ViolaJonesOutputWithStatistic.h"


class ViolaJonesClassifier
{
public:
	ViolaJonesClassifier();
	ViolaJonesClassifier(int wnd_width, int wnd_height, const CascadeClassifier& cascade_classifier, const StatMomentsClassifier& prefilter_classifier,
						ClassificationTag correct_tag, ClassificationTag incorrect_tag);

	ViolaJonesClassifier(const ViolaJonesClassifier& other);
	ViolaJonesClassifier(ViolaJonesClassifier&& other);
	ViolaJonesClassifier& operator=(const ViolaJonesClassifier& other);
	ViolaJonesClassifier& operator=(ViolaJonesClassifier&& other);

	std::vector<Rectangle> apply(const IntegralImage& image, int min_object_size, int max_object_size, double scale_factor, double step_factor) const;
	inline std::vector<Rectangle> apply(const IntegralImage& image) const
	{
		return apply(image, -1, -1, -1, -1);
	}
	ViolaJonesOutputWithStatistic apply_with_statistic(const IntegralImage& image, int min_object_size, int max_object_size, double scale_factor, double step_factor) const;
	inline ViolaJonesOutputWithStatistic apply_with_statistic(const IntegralImage& image) const
	{
		return apply_with_statistic(image, -1, -1, -1, -1);
	}

	inline StatMomentsClassifier get_prefilter_classifier() const
	{
		return _prefilter_classifier;
	}
	inline void set_prefilter_classifier(const StatMomentsClassifier& prefilter_classifier)
	{
		_prefilter_classifier = prefilter_classifier;
	}

	inline const CascadeClassifier& get_cascade_classifier() const
	{
		return _cascade_classfifer;
	}

	inline void update_scale_ranges()
	{
		update_scale_ranges(-1, -1, -1);
	}

	inline int get_wnd_width() const
	{
		return _wnd_width;
	}

	inline int get_wnd_height() const
	{
		return _wnd_height;
	}

	inline double get_scale_from() const
	{
		return _scale_from;
	}

	inline double get_scale_to() const
	{
		return _scale_to;
	}

	inline double get_scale_step() const
	{
		return _scale_step;
	}

	void update_scale_ranges(double from, double to, double step);

	inline ClassificationTag get_correct_tag() const
	{
		return _correct_tag;
	}

	inline ClassificationTag get_incorrect_tag() const
	{
		return _incorrect_tag;
	}

private:
	StatMomentsClassifier _prefilter_classifier;
	CascadeClassifier _cascade_classfifer;
	int _wnd_width;
	int _wnd_height;

	std::vector<ScaledCascadeInfoCompact> _scaled_cascade;
	double _scale_from;
	double _scale_to;
	double _scale_step;
	std::vector<int> _cumulative_weak_cls_by_stage;

	ClassificationTag _correct_tag;
	ClassificationTag _incorrect_tag;

	static const int _pixel_extent_by_scale;
	static const int _max_image_size;

	void update_statistic_data();

	void scale_correction(const IntegralImage& image, int min_object_size, int max_object_size, double scale_factor, double &scale_from_out, double &scale_to_out, double &scale_step_out) const;
};