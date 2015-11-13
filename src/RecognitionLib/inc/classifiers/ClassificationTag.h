#pragma once

class ClassificationTag
{
public:
	ClassificationTag()
	{
		_tag = 0;
	}
	ClassificationTag(int tag)
	{
		_tag = tag;
	}

	inline int value() const
	{
		return _tag;
	}

	inline bool is_negative() const
	{
		return _tag == 0;
	}
	inline bool is_positive() const
	{
		return _tag == 1;
	}
	inline bool is_custom() const
	{
		return _tag >= 2;
	}

	bool operator != (const ClassificationTag& other) const
	{
		return this->_tag != other._tag;
	}
	bool operator == (const ClassificationTag& other) const
	{
		return this->_tag == other._tag;
	}

private:
	int _tag;
};