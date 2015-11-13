#pragma once
#include <memory>


template<class T>
class OwnTrackingPtr
{
public:
	OwnTrackingPtr()
	{
		_asShared = std::shared_ptr<T>(nullptr);
		_asRef = nullptr;
	}
	OwnTrackingPtr(std::nullptr_t)
	{
		_asShared = std::shared_ptr<T>(nullptr);
		_asRef = nullptr;
	}
	explicit OwnTrackingPtr(const std::shared_ptr<T>& src)
	{
		_asShared = src;
		_asRef = _asShared.get();
	}
	explicit OwnTrackingPtr(T* src)
	{
		_asShared = std::shared_ptr<T>(nullptr);
		_asRef = src;
	}
	OwnTrackingPtr(T* src, bool isOwn)
	{
		if (isOwn)
			_asShared = std::shared_ptr<T>(src);
		else
			_asShared = std::shared_ptr<T>(nullptr);
		_asRef = src;
	}
	OwnTrackingPtr(const OwnTrackingPtr<T>& src)
	{
		_asShared = src._asShared;
		_asRef = src._asRef;
	}
	OwnTrackingPtr(OwnTrackingPtr<T>&& src)
	{
		_asShared = std::move(src._asShared);
		_asRef = src._asRef;

		src._asShared = std::shared_ptr<T>(nullptr);
		src._asRef = nullptr;
	}

	OwnTrackingPtr<T>& operator=(const OwnTrackingPtr<T>& src)
	{
		if (this == &src)
			return *this;
		this->_asShared = src._asShared;
		this->_asRef = src._asRef;
		return *this;
	}

	OwnTrackingPtr<T>& operator=(OwnTrackingPtr<T>&& src)
	{
		if (this == &src)
			return *this;
			
		_asShared = std::move(src._asShared);
		_asRef = src._asRef;

		src._asShared = std::shared_ptr<T>(nullptr);
		src._asRef = nullptr;

		return *this;
	}

	~OwnTrackingPtr()
	{
		reset();
	}

	T* get() const
	{
		return _asRef;
	}

	bool isOwns() const
	{
		return _asShared.get() != nullptr;
	}

	void reset()
	{
		_asRef = nullptr;
		_asShared.reset();
	}

	T& operator*() const
	{
		return *_asRef;
	}

	T* operator->() const
	{
		return _asRef;
	}

	operator bool() const
	{
		return _asRef != nullptr;
	}

private:
	std::shared_ptr<T> _asShared;
	T* _asRef;
};
