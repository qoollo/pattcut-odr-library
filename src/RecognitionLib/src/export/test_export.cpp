#include "inc/export/test_export.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include "inc/images/ImageLoader.h"
#include "inc/tests/test_integral_image.h"

#include "inc/classifier_loaders/ClassifierLoader.h"
#include "inc/classifier_loaders/ClassifierLoaderOpenCV.h"



testExport::testExport()
{

}

testExport::~testExport()
{

}
void testExport::method()
{
	printf("TestExport Programm Started: ");
}



bool testExport::test_load_image(const void* data, const std::size_t length, int& width, int& height, void* out_buffer, std::size_t& out_buffer_length)
{
	try
	{
		auto img = ImageLoader::load(data, length);
		width = img.get_width();
		height = img.get_height();

		out_buffer_length = ImageLoader::save_bmp(img, out_buffer, out_buffer_length);

		return true;
	}
	catch (ImageLoadingException&)
	{
		return false;
	}
}



void testExport::call_test_integral_image()
{
	test_integral_image();
}

void testExport::test_xml(const std::string& str)
{
	std::string line;
	std::string res_line = "";
	std::ifstream myfile(str);
	ClassificationTag positive_tag = ClassificationTag(1);
	ClassificationTag negative_tag = ClassificationTag(0);
	if (myfile.is_open())
	{
		while (myfile.good())
		{
			getline(myfile, line);
			res_line += line;
		}
		myfile.close();
	}
//	ClassifierLoader::load_from_xml_string(res_line, positive_tag, negative_tag);
	ClassifierLoaderOpenCV::load_from_file(str, positive_tag, negative_tag);
}

void testExport::test_load_image_perf(const std::string & file_path, int iter_num)
{
	std::fstream file;
	file.open(file_path, std::ios_base::in | std::ios_base::binary);
	file.seekg(0, std::ios_base::end);

	size_t size = (size_t)file.tellg();
	std::unique_ptr<uint8_t[]> arr = std::unique_ptr<uint8_t[]>(new uint8_t[size]);
	
	file.seekg(0);
	file.read((char*)arr.get(), size);
	file.close();

	Image img = Image();//ImageLoader::load_with_image_magic(arr.get(), size);

	size_t save_arr_size = 1024 + img.get_width() * img.get_height() * 4;
	std::unique_ptr<uint8_t[]> save_arr = std::unique_ptr<uint8_t[]>(new uint8_t[save_arr_size]);
	size_t finalSize = ImageLoader::save_bmp(img, save_arr.get(), save_arr_size);

	file.open(file_path + ".bmp", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	file.write((char*)save_arr.get(), finalSize);
	file.close();


	int variable = 0;

	auto clockStart = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < iter_num; i++)
	{
		img = ImageLoader::load(arr.get(), size);
		variable += img.get_width();
	}

	auto diff = std::chrono::high_resolution_clock::now() - clockStart;
	std::cout << "Our Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms. IterCount: " << iter_num << ". Var: " << variable << std::endl;

	
	//variable = 0;
	//clockStart = std::chrono::high_resolution_clock::now();

	//for (int i = 0; i < iter_num; i++)
	//{
	//	img = ImageLoader::load_with_image_magic(arr.get(), size);
	//	variable += img.get_width();
	//}

	//diff = std::chrono::high_resolution_clock::now() - clockStart;
	//std::cout << "Magick Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms. IterCount: " << iter_num << ". Var: " << variable << std::endl;
}


