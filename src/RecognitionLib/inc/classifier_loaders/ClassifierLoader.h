#pragma once
#include <string>
#include "inc/classifiers/ClassificationTag.h"
#include "inc/classifiers/viola_jones/ViolaJonesClassifier.h"
#include "inc/support/Exceptions.h"

class ClassifierLoader
{
public:
	static ViolaJonesClassifier load_from_file(const std::string& file_name, ClassificationTag positive_tag, ClassificationTag negative_tag);

	static ViolaJonesClassifier load_from_xml_string(const std::string& xml_data, ClassificationTag positive_tag, ClassificationTag negative_tag);
};