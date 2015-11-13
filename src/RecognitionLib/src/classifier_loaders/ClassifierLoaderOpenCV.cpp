#include <limits>
#include <vector>
#include <locale.h>
#include "inc/classifier_loaders/ClassifierLoaderOpenCV.h"
#include "inc/TinyXML/tinyxml.h"
#include "inc/TinyXML/tinystr.h"
#include "inc/classifiers/simple/StatMomentsClassifier.h"
#include "inc/classifiers/simple/StumpClassifier.h"
#include "inc/classifiers/cascade/CascadeClassifier.h"
#include "inc/classifiers/boosted/BoostedClassifier.h"
#include "inc/haar_features/HaarFeature.h"
#include "inc/haar_features/EvaluateRactangle.h"

static ViolaJonesClassifier load_from_xml_opencv(TiXmlDocument& reader, ClassificationTag positive_tag, ClassificationTag negative_tag);

static ViolaJonesClassifier load_vj_controller(TiXmlElement* first_element, ClassificationTag positive_tag, ClassificationTag negative_tag);

static StatMomentsClassifier load_prefilter(ClassificationTag positive_tag, ClassificationTag negative_tag);

static CascadeClassifier load_cascade(TiXmlElement* element, const std::vector<HaarFeature>& vec_features, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height);

static BoostedClassifier load_boosted_classifier(TiXmlElement* element, const std::vector<HaarFeature>& vec_features, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height);

static StumpClassifier load_weak_classifier(TiXmlElement* current_weak_classifier, const std::vector<HaarFeature>& vec_features, double& threshold_correction, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height);

static HaarFeature load_feature(TiXmlElement* element, int wnd_width, int wnd_height);

static std::vector<HaarFeature> load_haar_features(TiXmlElement *element, int wnd_width, int wnd_height);

static EvaluateRactangle load_evaluate_rectangle(TiXmlElement* element, int wnd_width, int wnd_height);


static std::vector<std::string> split(const std::string &s, char c)
{
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);
	std::vector<std::string> v;

	while (j != std::string::npos) {
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::string::npos)
		v.push_back(s.substr(i, s.length()));
	}
	return v;
}

static double str_to_double(const std::string& str)
{
	struct lconv * lc = localeconv();
	std::string str_for_swap = str;
	if (lc->decimal_point[0] == '.')
		std::replace(str_for_swap.begin(), str_for_swap.end(), ',', '.');
	else if (lc->decimal_point[0] == ',')
		std::replace(str_for_swap.begin(), str_for_swap.end(), '.', ',');

	size_t pos;
	double convertion_result = std::stod(str_for_swap, &pos);
	if (pos != str_for_swap.length())
		throw ClassifierLoadingException(std::string("string '") + str_for_swap + "' was not completely converted to double");
	return convertion_result;
}

ViolaJonesClassifier ClassifierLoaderOpenCV::load_from_file(const std::string& file_name, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	TiXmlDocument doc;
	if (doc.LoadFile(file_name) == false)
		throw ClassifierLoadingException(std::string("file did not load, reason: ") + doc.ErrorDesc());
	return load_from_xml_opencv(doc, positive_tag, negative_tag);
}

ViolaJonesClassifier ClassifierLoaderOpenCV::load_from_xml_string(const std::string& xml_data, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	TiXmlDocument xml_doc;
	xml_doc.Parse(xml_data.c_str());
	if (xml_doc.Error())
		throw ClassifierLoadingException(std::string("file did not load, reason: ") + xml_doc.ErrorDesc());
	return load_from_xml_opencv(xml_doc, positive_tag, negative_tag);
}

static ViolaJonesClassifier load_from_xml_opencv(TiXmlDocument& doc, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	auto storage_element = doc.FirstChildElement("opencv_storage");
	if (storage_element == nullptr)
		throw ClassifierLoadingException("'opencv_storage' element not found");
	auto cascade_element = storage_element->FirstChildElement("cascade");
	if (cascade_element == nullptr)
		throw ClassifierLoadingException("'cascade_element' element not found");

	auto stage_type_element = cascade_element->FirstChildElement("stageType");
	if (stage_type_element == nullptr)
		throw ClassifierLoadingException("'stageType' element not found");
	if (stage_type_element->GetText() == nullptr)
		throw ClassifierLoadingException("'stageType' field is not text");
	std::string str = stage_type_element->GetText();
	if (str != "BOOST")
		throw ClassifierLoadingException(std::string("wrong stageType: '") + str + "', expected 'BOOST' ");

	auto feature_type_element = cascade_element->FirstChildElement("featureType");
	if (feature_type_element == nullptr)
		throw ClassifierLoadingException("'featureType' element not found");
	if (feature_type_element->GetText() == nullptr)
		throw ClassifierLoadingException("'featureType' field is not text");
	str = feature_type_element->GetText();
	if (str != "HAAR")
		throw ClassifierLoadingException(std::string("wrong featureType: '") + str + "', expected 'HAAR' ");
	return load_vj_controller(cascade_element, positive_tag, negative_tag);
}

static ViolaJonesClassifier load_vj_controller(TiXmlElement* first_element, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	if (first_element == nullptr || first_element->ValueStr() != "cascade")
		throw ClassifierLoadingException("'cascade' element not found");

	auto prefilter = load_prefilter(positive_tag, negative_tag);

	if (first_element->FirstChildElement("height") == nullptr)
		throw ClassifierLoadingException("'height' element not found");
	if (first_element->FirstChildElement("height")->GetText() == nullptr)
		throw ClassifierLoadingException("'heigh' element's' field is not a text");
	auto wnd_height = std::stoi(first_element->FirstChildElement("height")->GetText());
	if (wnd_height <= 0)
		throw ClassifierLoadingException("'height' should be positive");

	if (first_element->FirstChildElement("width") == nullptr)
		throw ClassifierLoadingException("'width' element not found");
	if (first_element->FirstChildElement("width")->GetText() == nullptr)
		throw ClassifierLoadingException("'width' element's' field is not a text");
	auto wnd_width = std::stoi(first_element->FirstChildElement("width")->GetText());
	if (wnd_width <= 0)
		throw ClassifierLoadingException("'width' should be positive");

	if (first_element->FirstChildElement("features") == nullptr)
		throw ClassifierLoadingException("'features' element not found");

	std::vector<HaarFeature> vec_features = load_haar_features(first_element->FirstChildElement("features"), wnd_width, wnd_height);

	CascadeClassifier cascade = load_cascade(first_element, vec_features, positive_tag, negative_tag, wnd_width, wnd_height);

	return ViolaJonesClassifier(wnd_width, wnd_height, cascade, prefilter, positive_tag, negative_tag);
}

static StatMomentsClassifier load_prefilter(ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	double min_variance = 1.0 / 1E-1;  // Const from OpenCV source code
	double max_variance = std::numeric_limits<double>::max() - 1;
	return StatMomentsClassifier(0.0, std::numeric_limits<double>::max() - 1, min_variance, max_variance, positive_tag, negative_tag);
}

static std::vector<HaarFeature> load_haar_features(TiXmlElement* element, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "features")
		throw ClassifierLoadingException("'feature' element expected");

	TiXmlElement* current_feature = element->FirstChildElement("_");
	if (current_feature == nullptr)
		throw ClassifierLoadingException("features '_' element expected");
	std::vector<HaarFeature> vec_features;

	while (current_feature != nullptr)
	{
		auto feature = load_feature(current_feature, wnd_width, wnd_height);
		current_feature = current_feature->NextSiblingElement("_");

		vec_features.push_back(feature);
	}
	return vec_features;
}

static HaarFeature load_feature(TiXmlElement* element, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "_")
		throw ClassifierLoadingException("'_' feature element not found");

	TiXmlElement* rects = element->FirstChildElement("rects");
	if (rects == nullptr)
		throw ClassifierLoadingException("'rects' element not found");

	TiXmlElement* tilted_element = rects->NextSiblingElement("tilted");
	if (tilted_element != nullptr)
	{
		if (tilted_element->GetText() == nullptr)
			throw ClassifierLoadingException("'tilted' element's field is not a text");
		int tilted_val = atoi(tilted_element->GetText());
		if ( tilted_val != 0 )
			throw ClassifierLoadingException("tilted features are not supported, 'tilted' must be 0");
	}

	std::vector<EvaluateRactangle> rectangles;
	TiXmlElement* rectangle = rects->FirstChildElement("_");
	if (rectangle == nullptr)
		throw ClassifierLoadingException("'_' rectangle element not found");

	while (rectangle!=nullptr)
	{
		rectangles.push_back(load_evaluate_rectangle(rectangle, wnd_width, wnd_height));
		rectangle = rectangle->NextSiblingElement("_");
	}

	if (rectangles.size() == 3)
		return HaarFeature(rectangles[0], rectangles[1], rectangles[2]);
	else if (rectangles.size() == 2)
		return HaarFeature(rectangles[0], rectangles[1]);
	else
		throw ClassifierLoadingException(std::string("wrong number of rectangles: ") + std::to_string(rectangles.size()) + ", expected 2 or 3");
}

static EvaluateRactangle load_evaluate_rectangle(TiXmlElement* element, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "_")
		throw ClassifierLoadingException("'_' rectangle element not found");

	if(element->GetText() == nullptr)
		throw ClassifierLoadingException("rectangle's field '_' is not a text");
	std::string str(element->GetText());
	std::vector<std::string> vec_values = split(str,' ');
	if (vec_values.size() != 5)
		throw ClassifierLoadingException(std::string("wrong rectangle string values: ") + str);

	int x_1 = std::stoi(vec_values[0]);
	if (x_1 < 0)
		throw ClassifierLoadingException("'x1' cannot be less than 0");
	if (x_1 > wnd_width)
		throw ClassifierLoadingException("'x1' cannot be greater than 'WndWidth'");

	int y_1 = std::stoi(vec_values[1]);
	if (y_1 < 0)
		throw ClassifierLoadingException("'y1' cannot be less than 0");
	if (y_1 > wnd_height)
		throw ClassifierLoadingException("'y1' cannot be greater than 'WndHeight'");

	int x_2 = x_1 + std::stoi(vec_values[2]);
	if (x_2 < x_1)
		throw ClassifierLoadingException("'x2' of rect should be >= 'x1'");
	if (x_2 > wnd_width)
		throw ClassifierLoadingException("'x2' cannot be greater than 'WndWidth'");

	int y_2 = y_1 + std::stoi(vec_values[3]);
	if (y_2 < y_1)
		throw ClassifierLoadingException("'y2' of rect should be >= 'y1'");
	if (y_2 > wnd_height)
		throw ClassifierLoadingException("'y2' cannot be greater than 'WndHeight'");

	double weight = str_to_double(vec_values[4]);
	if ((weight - (int)weight) != 0)
		throw ClassifierLoadingException(std::string("weight = ") + std::to_string(weight) + ", excepted integer value");
	return EvaluateRactangle(x_1, y_1, x_2, y_2, (int)weight);
}

static CascadeClassifier load_cascade(TiXmlElement* element, const std::vector<HaarFeature>& vec_features, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "cascade")
		throw ClassifierLoadingException("'cascade' element expected");

	TiXmlElement* stages_element;
	if ((stages_element = element->FirstChildElement("stages")) == nullptr)
		throw ClassifierLoadingException("'stages' element not found");

	TiXmlElement* boosted_element;
	if ((boosted_element = stages_element->FirstChildElement("_")) == nullptr)
		throw ClassifierLoadingException("'_' first boosted element of stages not found");

	std::vector<BoostedClassifier> boosted_classifiers;

	while (boosted_element != nullptr)
	{
		auto strong_clsf = load_boosted_classifier(boosted_element, vec_features, positive_tag, negative_tag, wnd_width, wnd_height);
		boosted_classifiers.push_back(strong_clsf);

		boosted_element = boosted_element->NextSiblingElement("_");
	}

	return CascadeClassifier(boosted_classifiers, positive_tag, negative_tag);
}

static BoostedClassifier load_boosted_classifier(TiXmlElement* element, const std::vector<HaarFeature>& vec_features, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "_")
		throw ClassifierLoadingException("'_' strong_classifier element not found");

	TiXmlElement* threshold;
	if ((threshold = element->FirstChildElement("stageThreshold")) == nullptr)
		throw ClassifierLoadingException("'stageThreshold' element not found");
	if (threshold->GetText() == nullptr)
		throw ClassifierLoadingException("'stageThreshold' element is empty");
	double stage_threshold = str_to_double(threshold->GetText());

	TiXmlElement* weak_classifiers = element->FirstChildElement("weakClassifiers");
	if (weak_classifiers == nullptr)
		throw ClassifierLoadingException("'weakClassifiers' element not found");

	TiXmlElement* current_weak_classifier = weak_classifiers->FirstChildElement("_");
	if (current_weak_classifier == nullptr)
		throw ClassifierLoadingException("'_' weak clissifier element not found");

	std::vector<StumpClassifier> vec_weak_classifiers;
	double threshold_correction = 0;

	while (current_weak_classifier != NULL)
	{
		auto weak_classifier = load_weak_classifier(current_weak_classifier, vec_features, threshold_correction, positive_tag, negative_tag, wnd_width, wnd_height);
		vec_weak_classifiers.push_back(weak_classifier);
		current_weak_classifier = current_weak_classifier->NextSiblingElement("_");
	}
	return BoostedClassifier(vec_weak_classifiers, stage_threshold + threshold_correction, positive_tag, negative_tag);
}

static StumpClassifier load_weak_classifier(TiXmlElement* current_weak_classifier, const std::vector<HaarFeature>& vec_features, double& threshold_correction, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height)
{
	if (current_weak_classifier == nullptr || current_weak_classifier->ValueStr() != "_")
		throw ClassifierLoadingException("'_' current weak classifier element not found");

	TiXmlElement* internal_nodes = current_weak_classifier->FirstChildElement("internalNodes");
	if (internal_nodes == nullptr)
		throw ClassifierLoadingException("'internalNodes' element not found");
	if(internal_nodes->GetText() == nullptr)
		throw ClassifierLoadingException("'internal_nodes' element's field is not a text");
	std::string str_internal(internal_nodes->GetText());
	std::vector<std::string> vec_internal = split(str_internal, ' ');

	if (vec_internal.size() != 4)
		throw ClassifierLoadingException(std::string("wrong internal nodes string: ") + str_internal);

	if(vec_internal[0] != "0" || vec_internal[1] != "-1")
		throw ClassifierLoadingException("the first element must be equal 0, second: -1");

	unsigned int feature_num = (unsigned int)std::stoi(vec_internal[2]);
	if ( feature_num >= vec_features.size())
		throw ClassifierLoadingException(std::string("number of feature: ") + std::to_string(feature_num) + " more than in the file: " + std::to_string(vec_features.size()));

	double weak_threshold = str_to_double(vec_internal[3]);
	weak_threshold = weak_threshold * (wnd_width * wnd_height); // OpenCV threshold correction

	TiXmlElement* leaf_values = current_weak_classifier->FirstChildElement("leafValues");
	if (leaf_values == nullptr)
		throw ClassifierLoadingException("'leafValue' element not found");
	if (leaf_values->GetText() == nullptr)
		throw ClassifierLoadingException("'leaf_values' element's field is not a text");
	std::string str_leaf(leaf_values->GetText());

	std::vector<std::string> vec_leaf = split(str_leaf, ' ');

	if (vec_leaf.size() != 2)
		throw ClassifierLoadingException(std::string("wrong leaf values string: ") + str_leaf);

	double less = str_to_double(vec_leaf[0]);
	double greater = str_to_double(vec_leaf[1]);

	if (less > 0 && greater > 0)
	{
		auto diff = fmin(less, greater);
		threshold_correction -= diff;
		less -= diff;
		greater -= diff;
	}
	else if (less < 0 && greater < 0)
	{
		auto diff = fmax(less, greater);
		threshold_correction -= diff;
		less -= diff;
		greater -= diff;
	}

	if (less >= 0 && greater <= 0)
		return StumpClassifier(vec_features[feature_num], weak_threshold, ClassificationResult(positive_tag, fabs(less)), ClassificationResult(negative_tag, fabs(greater)));
	else
		return StumpClassifier(vec_features[feature_num], weak_threshold, ClassificationResult(negative_tag, fabs(less)), ClassificationResult(positive_tag, fabs(greater)));
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-