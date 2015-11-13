#pragma once
#include <assert.h>
#include "inc/classifiers/ClassificationResult.h"
#include "inc/classifiers/ClassificationTag.h"
#include "inc/images/WindowIntegralImage.h"
#include "inc/classifiers/simple/FixedAreaStatMomentsClassifier.h"


class StatMomentsClassifier
{
public:
	StatMomentsClassifier();
	StatMomentsClassifier(double mean_min, double mean_max, double var_min, double var_max, ClassificationTag correct_tag, ClassificationTag incorrect_tag);
	StatMomentsClassifier(float mean_min, float mean_max, float var_min, float var_max, ClassificationTag correct_tag, ClassificationTag incorrect_tag);
    
	inline float get_mean_min() const
    {
        return _mean_min;
    }
	inline void set_mean_min(float mean_min)
    {
        _mean_min = mean_min;
    }
    
	inline float get_mean_max() const
    {
        return _mean_max;
    }
	inline void set_mean_max(float mean_max)
    {
        _mean_max = mean_max;
    }
    
	inline float get_var_min() const
    {
        return _var_min;
    }
	inline void set_var_min(float var_min)
    {
        _var_min = var_min;
    }
    
	inline float get_var_max() const
    {
        return _var_max;
    }
	inline void set_var_max(float var_max)
    {
        _var_max = var_max;
    }
    
    inline ClassificationTag get_correct_tag() const
    {
        return _correct_tag;
    }
    inline void set_correct_tag(ClassificationTag correct_tag)
    {
        _correct_tag = correct_tag;
    }
    
    inline ClassificationTag get_incorrect_tag() const
    {
        return _incorrect_tag;
    }
    inline void set_incorrect_tag(ClassificationTag incorrect_tag)
    {
        _incorrect_tag = incorrect_tag;
    }


	inline bool evaluate_bool(const WindowIntegralImage &img) const
	{
		return	
			img.get_variance() >= _var_min &&
			img.get_mean() >= _mean_min &&
			img.get_mean() <= _mean_max &&
			img.get_variance() <= _var_max;
	}
    
    inline ClassificationResult evaluate(const WindowIntegralImage &img) const
    {
		return evaluate_bool(img) ? ClassificationResult(_correct_tag) : ClassificationResult(_incorrect_tag);
    }   
    inline ClassificationTag evaluate_tag(const WindowIntegralImage &img) const
    {
		return evaluate_bool(img) ? _correct_tag : _incorrect_tag;
    }
    
	FixedAreaStatMomentsClassifier create_fixed_for_area(int width, int height) const;
	StatMomentsClassifier copy_scaled(double ratio) const;

    
private:
	float _mean_min, _mean_max;
	float _var_min, _var_max;
    ClassificationTag _correct_tag;
    ClassificationTag _incorrect_tag;
};