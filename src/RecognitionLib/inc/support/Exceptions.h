#pragma once
#include <stdexcept>
#include <string>

class LibraryException : public std::runtime_error
{
public:
	LibraryException(const std::string& error)
		: std::runtime_error(error)
	{
	}
};


class ImageLoadingException : public LibraryException
{
public:
	ImageLoadingException(const std::string& error)
		: LibraryException(error)
	{
	} 
};

class ClassifierLoadingException : public LibraryException
{
public:
	ClassifierLoadingException(const std::string& error)
		: LibraryException(error)
	{
	}
};


class InvalidScaleException : public LibraryException
{
public:
	InvalidScaleException(const std::string& error)
		: LibraryException(error)
	{
	}
};