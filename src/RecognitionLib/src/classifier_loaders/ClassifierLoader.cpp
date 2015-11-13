#include "inc/classifier_loaders/ClassifierLoader.h"
#include "inc/classifier_loaders/ClassifierLoaderPattCut.h"
#include "inc/classifier_loaders/ClassifierLoaderOpenCV.h"
#include <fstream>



static std::vector<bool> find_strings_in_file(const std::string& file_name, const std::vector<std::string>& search_for)
{
	std::vector<bool> result;
	result.resize(search_for.size(), false);

	std::fstream file;
	file.open(file_name, std::ios_base::in);

	if (!file.is_open())
	{
		throw ClassifierLoadingException(std::string("Classifier file was not found: ") + file_name);
	}
	if (file.bad())
	{
		file.close();
		throw ClassifierLoadingException(std::string("Error during open classifier file: ") + file_name);
	}


	std::string line_data;

	while (!file.eof())
	{
		line_data.clear();
		std::getline(file, line_data);

		for (size_t i = 0; i < search_for.size(); i++)
		{
			if (result[i] == false && line_data.find(search_for[i]) != std::string::npos)
				result[i] = true;
		}
	}
	

	if (file.bad())
	{
		file.close();
		throw ClassifierLoadingException(std::string("Error during classifier file reading: ") + file_name);
	}

	file.close();

	return result;
}



ViolaJonesClassifier ClassifierLoader::load_from_file(const std::string& file_name, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	std::vector<std::string> string_to_search = { "<opencv_storage>", "<cascades" };
	auto string_located = find_strings_in_file(file_name, string_to_search);

	if (string_located[0])
		return ClassifierLoaderOpenCV::load_from_file(file_name, positive_tag, negative_tag);

	if (string_located[1])
		return ClassifierLoaderPattCut::load_from_file(file_name, positive_tag, negative_tag);

	throw ClassifierLoadingException(std::string("Unknown classifier file format. Check your classifier file: ") + file_name);
}



ViolaJonesClassifier ClassifierLoader::load_from_xml_string(const std::string& xml_data, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	if (xml_data.find("<opencv_storage>") != std::string::npos)
		return ClassifierLoaderOpenCV::load_from_xml_string(xml_data, positive_tag, negative_tag);

	if (xml_data.find("<cascades") != std::string::npos)
		return ClassifierLoaderPattCut::load_from_xml_string(xml_data, positive_tag, negative_tag);

	throw ClassifierLoadingException(std::string("Unknown classifier format"));
}