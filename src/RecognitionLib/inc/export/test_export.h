#pragma once
#include "pattcut/export_helper.h"
#include <cstddef>
#include <string>

class DLL_EXPORT_TEST testExport
{
public:
	testExport();
	~testExport();

	void method();

	
    bool test_load_image(const void* data, const std::size_t length, int& width, int& height, void* out_buffer, std::size_t& out_buffer_length);
    
	void call_test_integral_image();

	void test_xml(const std::string& str);

	void test_load_image_perf(const std::string& file_path, int iter_num);
};