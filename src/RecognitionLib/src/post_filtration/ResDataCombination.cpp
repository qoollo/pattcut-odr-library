#include "inc/post_filtration/ResDataCombination.h"
#include <limits>

ResDataCombination::ResDataCombination()
{
	_count = 0;
	_aggregated_count = 0;

	_last_boosted_classifier_value_max = 0;
	_last_boosted_classifier_value_min = 0;
	_last_boosted_classifier_value_aggregation = 0;

	_comb_boosted_classifier_value_max = 0;
	_comb_boosted_classifier_value_min = 0;
	_comb_boosted_classifier_value_aggregation = 0;
}

ResDataCombination::ResDataCombination(const ResDataCombination& init_data)
{
	_average_area = init_data._average_area;
	_bound_area = init_data._bound_area;
	_count = init_data._count;
	_aggregated_count = init_data._aggregated_count;
	_aggregated_area = init_data._aggregated_area;

	_last_boosted_classifier_value_max = init_data._last_boosted_classifier_value_max;
	_last_boosted_classifier_value_min = init_data._last_boosted_classifier_value_min;
	_last_boosted_classifier_value_aggregation = init_data._last_boosted_classifier_value_aggregation;

	_comb_boosted_classifier_value_max = init_data._comb_boosted_classifier_value_max;
	_comb_boosted_classifier_value_min = init_data._comb_boosted_classifier_value_min;
	_comb_boosted_classifier_value_aggregation = init_data._comb_boosted_classifier_value_aggregation;
}

ResDataCombination ResDataCombination::as_new_area(const ResDataCombination& init_data)
{
	ResDataCombination res;

	res._average_area = init_data._average_area;
	res._bound_area = init_data._average_area;
	res._aggregated_area = RectangleF::from_rectangle(init_data._average_area);
	res._count = init_data._count;
	res._aggregated_count = 1;

	res._last_boosted_classifier_value_max = init_data._last_boosted_classifier_value_max;
	res._last_boosted_classifier_value_min = init_data._last_boosted_classifier_value_min;
	res._last_boosted_classifier_value_aggregation = init_data._last_boosted_classifier_value_aggregation;

	res._comb_boosted_classifier_value_max = init_data._comb_boosted_classifier_value_max;
	res._comb_boosted_classifier_value_min = init_data._comb_boosted_classifier_value_min;
	res._comb_boosted_classifier_value_aggregation = init_data._comb_boosted_classifier_value_aggregation;

	return res;
}

ResDataCombination ResDataCombination::from_rectangle(const Rectangle& init_data)
{
	ResDataCombination res;

	res._average_area = init_data;
	res._bound_area = init_data;
	res._aggregated_area = RectangleF::from_rectangle(init_data);
	res._count = 1;
	res._aggregated_count = 1;
	res._last_boosted_classifier_value_max = std::numeric_limits<double>::lowest();
	res._last_boosted_classifier_value_min = std::numeric_limits<double>::max();
	res._last_boosted_classifier_value_aggregation = 0;

	res._comb_boosted_classifier_value_max = std::numeric_limits<double>::lowest();
	res._comb_boosted_classifier_value_min = std::numeric_limits<double>::max();
	res._comb_boosted_classifier_value_aggregation = 0;

	return res;
}

ResDataCombination ResDataCombination::from_area_with_ext_info(const AreaWithExtInfo& init_data)
{
	ResDataCombination res;

	res._average_area = init_data.get_area();
	res._bound_area = init_data.get_area();
	res._aggregated_area = RectangleF::from_rectangle(init_data.get_area());
	res._count = 1;
	res._aggregated_count = 1;

	res._last_boosted_classifier_value_max = init_data.get_last_boosted_classifier_result().confidence();
	res._last_boosted_classifier_value_min = init_data.get_last_boosted_classifier_result().confidence();
	res._last_boosted_classifier_value_aggregation = init_data.get_last_boosted_classifier_result().confidence();

	res._comb_boosted_classifier_value_max = init_data.get_comd_boosted_classifier_value();
	res._comb_boosted_classifier_value_min = init_data.get_comd_boosted_classifier_value();
	res._comb_boosted_classifier_value_aggregation = init_data.get_comd_boosted_classifier_value();

	return res;
}

void ResDataCombination::include_area(const ResDataCombination &area)
{
	this->_bound_area = Rectangle::combine(_bound_area, area._average_area);

	this->_aggregated_area.left() += area._average_area.left();
	this->_aggregated_area.top() += area._average_area.top();
	this->_aggregated_area.width() += area._average_area.width();
	this->_aggregated_area.height() += area._average_area.height();

	this->_aggregated_count++;
	this->_count += area._count;

	this->_last_boosted_classifier_value_max = std::max(area._last_boosted_classifier_value_max, this->_last_boosted_classifier_value_max);
	this->_last_boosted_classifier_value_min = std::min(area._last_boosted_classifier_value_min, this->_last_boosted_classifier_value_min);
	this->_last_boosted_classifier_value_aggregation += area._last_boosted_classifier_value_aggregation;

	this->_comb_boosted_classifier_value_max = std::max(area._comb_boosted_classifier_value_max, this->_comb_boosted_classifier_value_max);
	this->_comb_boosted_classifier_value_min = std::min(area._comb_boosted_classifier_value_min, this->_comb_boosted_classifier_value_min);
	this->_comb_boosted_classifier_value_aggregation += area._comb_boosted_classifier_value_aggregation;
}

void ResDataCombination::include_area_proportional(const ResDataCombination& area, double priority)
{
	this->_bound_area = Rectangle::combine(_bound_area, area._bound_area);

	this->_aggregated_area.left() += area._aggregated_area.left() * priority;
	this->_aggregated_area.top() += area._aggregated_area.top() * priority;
	this->_aggregated_area.width() += area._aggregated_area.width() * priority;
	this->_aggregated_area.height() += area._aggregated_area.height() * priority;

	this->_aggregated_count += area._aggregated_count * priority;
	this->_count += area._count;

	this->_last_boosted_classifier_value_max = std::max(area._last_boosted_classifier_value_max, this->_last_boosted_classifier_value_max);
	this->_last_boosted_classifier_value_min = std::min(area._last_boosted_classifier_value_min, this->_last_boosted_classifier_value_min);
	this->_last_boosted_classifier_value_aggregation += area._last_boosted_classifier_value_aggregation;

	this->_comb_boosted_classifier_value_max = std::max(area._comb_boosted_classifier_value_max, this->_comb_boosted_classifier_value_max);
	this->_comb_boosted_classifier_value_min = std::min(area._comb_boosted_classifier_value_min, this->_comb_boosted_classifier_value_min);
	this->_comb_boosted_classifier_value_aggregation += area._comb_boosted_classifier_value_aggregation;
}

void ResDataCombination::update_average()
{
	_average_area = Rectangle(
		(int)(_aggregated_area.left() / _aggregated_count),
		(int)(_aggregated_area.top() / _aggregated_count),
		(int)(_aggregated_area.width() / _aggregated_count),
		(int)(_aggregated_area.height() / _aggregated_count));
}