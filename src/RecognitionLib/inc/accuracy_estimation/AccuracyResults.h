#pragma once

class AccuracyResults
{
public:
	AccuracyResults()
	{
		_accuracy = 0;
		_is_good = false;
	}

	AccuracyResults(double accuracy)
	{
		_accuracy = accuracy;

		_is_good = accuracy > 0;
	}

	AccuracyResults(double accuracy, bool is_good)
	{
		_accuracy = accuracy;
		_is_good = is_good;
	}

	inline double accuracy() const
	{
		return _accuracy;
	}

	inline bool is_good() const
	{
		return _is_good;
	}

private:
	double _accuracy;
	bool _is_good;
};