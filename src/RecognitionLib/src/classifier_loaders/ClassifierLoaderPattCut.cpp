#include <limits>
#include <vector>
#include <locale.h>
#include "inc/classifier_loaders/ClassifierLoaderPattCut.h"
#include "inc/TinyXML/tinyxml.h"
#include "inc/TinyXML/tinystr.h"
#include "inc/classifiers/simple/StatMomentsClassifier.h"
#include "inc/classifiers/simple/StumpClassifier.h"
#include "inc/classifiers/cascade/CascadeClassifier.h"
#include "inc/classifiers/boosted/BoostedClassifier.h"
#include "inc/haar_features/HaarFeature.h"
#include "inc/haar_features/EvaluateRactangle.h"

static ViolaJonesClassifier load_from_xml(TiXmlDocument& reader, ClassificationTag positive_tag, ClassificationTag negative_tag);

static ViolaJonesClassifier load_vj_controller(TiXmlElement* first_element, ClassificationTag positive_tag, ClassificationTag negative_tag);

static StatMomentsClassifier load_prefilter(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag);

static CascadeClassifier load_cascade(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height);

static BoostedClassifier load_boosted_classifier(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height);

static StumpClassifier load_weak_classifier(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height);

static HaarFeature load_haar_feature(TiXmlElement* element, int wnd_width, int wnd_height);

static EvaluateRactangle load_evaluate_rectangle(TiXmlElement* element, int wnd_width, int wnd_height);


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


ViolaJonesClassifier ClassifierLoaderPattCut::load_from_file(const std::string& file_name, ClassificationTag positive_tag, ClassificationTag negative_tag)
{	
	TiXmlDocument doc;
	if (doc.LoadFile(file_name) == false)
		throw ClassifierLoadingException(std::string("file did not load, reason: ") + doc.ErrorDesc());
	return load_from_xml(doc, positive_tag, negative_tag);
}

ViolaJonesClassifier ClassifierLoaderPattCut::load_from_xml_string(const std::string& xml_data, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	TiXmlDocument xml_doc;
	xml_doc.Parse(xml_data.c_str());
	if (xml_doc.Error())
		throw ClassifierLoadingException(std::string("file did not load, reason: ") + xml_doc.ErrorDesc());
	return load_from_xml(xml_doc, positive_tag, negative_tag);
}

static ViolaJonesClassifier load_from_xml(TiXmlDocument& reader, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	auto cascade_element = reader.FirstChildElement("cascades");
	return load_vj_controller(cascade_element, positive_tag, negative_tag);
}

	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

static ViolaJonesClassifier load_vj_controller(TiXmlElement* first_element, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	if (first_element == nullptr || first_element->ValueStr() != "cascades")
		throw ClassifierLoadingException("'cascades' element not found");
	auto prefilter = load_prefilter(first_element, positive_tag, negative_tag);

	if (first_element->Attribute("WndWidth") == nullptr)
		throw ClassifierLoadingException("'WndWidth' attribute not found");
	auto wnd_width = std::stoi(first_element->Attribute("WndWidth"));
	if (wnd_width <= 0)
		throw ClassifierLoadingException("'WndWidth' should be positive");

	if (first_element->Attribute("WndHeight") == nullptr)
		throw ClassifierLoadingException("'WndHeight' attribute not found");
	auto wnd_height = std::stoi(first_element->Attribute("WndHeight"));
	if (wnd_height <= 0)
		throw ClassifierLoadingException("'WndHeight' should be positive");


	auto cascade = load_cascade(first_element, positive_tag, negative_tag, wnd_width, wnd_height);

	return ViolaJonesClassifier(wnd_width, wnd_height, cascade, prefilter, positive_tag, negative_tag);
}

static StatMomentsClassifier load_prefilter(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag)
{
	if (element == nullptr || element->ValueStr() != "cascades")
		throw ClassifierLoadingException("'cascades' element not found");

	const char* tmp = nullptr;
	double mean_min;
	double mean_max;
	double var_min;

	tmp = element->Attribute("MinMean");
	mean_min = tmp == nullptr ? 0.0 : str_to_double(tmp);

	tmp = element->Attribute("MaxMean");
	mean_max = tmp == nullptr ? std::numeric_limits<double>::max() : str_to_double(tmp);

	tmp = element->Attribute("MinVariance");
	var_min = tmp == nullptr ? 0.0 : str_to_double(tmp);

	return StatMomentsClassifier(mean_min, mean_max, var_min, std::numeric_limits<double>::max(), positive_tag, negative_tag);
}

static CascadeClassifier load_cascade(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height)
{
	if (element == nullptr)
		throw ClassifierLoadingException("'cascade' element expected");
	TiXmlElement* boosted_element;
	if ((boosted_element = element->FirstChildElement("strong_classifier")) == nullptr)
		throw ClassifierLoadingException("'strong_classifier' element not found");

	std::vector<BoostedClassifier> boosted_classifiers;

	while (boosted_element != nullptr)
	{
		auto strng_cls = load_boosted_classifier(boosted_element, positive_tag, negative_tag, wnd_width, wnd_height);
		if (strng_cls.get_classifier_count() > 0)
			boosted_classifiers.push_back(strng_cls);

		boosted_element = boosted_element->NextSiblingElement("strong_classifier");
	}

	return CascadeClassifier(boosted_classifiers, positive_tag, negative_tag);
}

static BoostedClassifier load_boosted_classifier(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "strong_classifier")
		throw ClassifierLoadingException("'strong_classifier' element not found");

	TiXmlElement* threshold;
	if ((threshold = element->FirstChildElement("threshold")) == nullptr)
		throw ClassifierLoadingException("'threshold' element not found");
	if (threshold->GetText() == nullptr)
		throw ClassifierLoadingException("'threshold' element is empty");
	double loc_threshold = 2 * str_to_double(threshold->GetText());

	TiXmlElement* weak_classifiers = element->FirstChildElement("WeakClassifiers");
	if (weak_classifiers == nullptr)
		throw ClassifierLoadingException("'WeakClassifiers' element not found");
	TiXmlElement* current_weak_classifier = weak_classifiers->FirstChildElement("weak_classifier");
	if (current_weak_classifier == nullptr)
		throw ClassifierLoadingException("'weak_classifier' element not found");

	std::vector<StumpClassifier> stump_classifiers;
	while (current_weak_classifier != NULL)
	{
		auto cur_wc = load_weak_classifier(current_weak_classifier, positive_tag, negative_tag, wnd_width, wnd_height);
		loc_threshold -= fabs(cur_wc.get_result_on_greater().confidence());
		if (fabs(cur_wc.get_result_on_greater().confidence()) < 1E+100)
			stump_classifiers.push_back(cur_wc);

		current_weak_classifier = current_weak_classifier->NextSiblingElement("weak_classifier");
	}

	return BoostedClassifier(stump_classifiers, loc_threshold, positive_tag, negative_tag);
}

static StumpClassifier load_weak_classifier(TiXmlElement* element, ClassificationTag positive_tag, ClassificationTag negative_tag, int wnd_width, int wnd_height)
{
	if (element == nullptr || element->ValueStr() != "weak_classifier")
		throw ClassifierLoadingException("'weak_classifier' element not found");

	const char* tmp = nullptr;

	tmp = element->Attribute("alpha");
	if (tmp == nullptr)
		throw ClassifierLoadingException("'alpha' attribite not found");
	double alpha = std::fabs(str_to_double(tmp));
	double raw_alpha = str_to_double(tmp);

	TiXmlElement* parity;
	if ((parity = element->FirstChildElement("parity")) == nullptr)
		throw ClassifierLoadingException("'parity' element not found");
	if (parity->GetText() == nullptr)
		throw ClassifierLoadingException("'parity' element is empty");
	int parity_value = std::stoi(parity->GetText());

	TiXmlElement* threshold;
	if ((threshold = element->FirstChildElement("threshold")) == nullptr)
		throw ClassifierLoadingException("'threshold' element not found");
	if (threshold->GetText() == nullptr)
		throw ClassifierLoadingException("'threshold' element is empty");
	double threshold_value = str_to_double(threshold->GetText());
	
	auto feature = load_haar_feature(element, wnd_width, wnd_height);

	if (raw_alpha * parity_value > 0)
		return StumpClassifier(feature, threshold_value, ClassificationResult(positive_tag, alpha), ClassificationResult(negative_tag, alpha));
	else
		return StumpClassifier(feature, threshold_value, ClassificationResult(negative_tag, alpha), ClassificationResult(positive_tag, alpha));
}

static HaarFeature load_haar_feature(TiXmlElement* element, int wnd_width, int wnd_height)
{
	if (element == nullptr)
		throw ClassifierLoadingException("'weak_classifier' element expected");

	TiXmlElement* rect_element_1;
	TiXmlElement* rect_element_2;
	TiXmlElement* rect_element_3;
	
	bool use_three_rect = false;

	if ((rect_element_1 = element->FirstChildElement("rect1")) == nullptr)
		throw ClassifierLoadingException("'rect1' element not found");
	EvaluateRactangle rect_1 = load_evaluate_rectangle(rect_element_1, wnd_width, wnd_height);

	if ((rect_element_2 = element->FirstChildElement("rect2")) == nullptr)
		throw ClassifierLoadingException("'rect2' element not found");
	EvaluateRactangle rect_2 = load_evaluate_rectangle(rect_element_2, wnd_width, wnd_height);

	EvaluateRactangle rect_3;
	if ((rect_element_3 = element->FirstChildElement("rect3")) != nullptr)
	{
		rect_3 = load_evaluate_rectangle(rect_element_3, wnd_width, wnd_height);
		use_three_rect = true;
	}
	
	return use_three_rect ? HaarFeature(rect_1, rect_2, rect_3) : HaarFeature(rect_1, rect_2);
}

static EvaluateRactangle load_evaluate_rectangle(TiXmlElement* element, int wnd_width, int wnd_height)
{
	if (element == nullptr)
		throw ClassifierLoadingException("'rect' element not found");

	if (element->Attribute("x1") == nullptr)
		throw ClassifierLoadingException("'x1' attribute not found");
	int x_1 = std::stoi(element->Attribute("x1"));
	if (x_1 < 0)
		throw ClassifierLoadingException("'x1' cannot be less than 0");
	if (x_1 > wnd_width)
		throw ClassifierLoadingException("'x1' cannot be greater than 'WndWidth'");

	if (element->Attribute("y1") == nullptr)
		throw ClassifierLoadingException("'y1' attribute not found");
	int y_1 = std::stoi(element->Attribute("y1"));
	if (y_1 < 0)
		throw ClassifierLoadingException("'y1' cannot be less than 0");
	if (y_1 > wnd_height)
		throw ClassifierLoadingException("'y1' cannot be greater than 'WndHeight'");

	if (element->Attribute("x2") == nullptr)
		throw ClassifierLoadingException("'x2' attribute not found");
	int x_2 = std::stoi(element->Attribute("x2"));
	if (x_2 < x_1)
		throw ClassifierLoadingException("'x2' of rect should be >= 'x1'");
	if (x_2 > wnd_width)
		throw ClassifierLoadingException("'x2' cannot be greater than 'WndWidth'");

	if (element->Attribute("y2") == nullptr)
		throw ClassifierLoadingException("'y2' attribute not found");
	int y_2 = std::stoi(element->Attribute("y2"));
	if (y_2 < y_1)
		throw ClassifierLoadingException("'y2' of rect should be >= 'y1'");
	if (y_2 > wnd_height)
		throw ClassifierLoadingException("'y2' cannot be greater than 'WndHeight'");

	if (element->Attribute("weight") == nullptr)
		throw ClassifierLoadingException("'weight' attribute not found");
	int weight = std::stoi(element->Attribute("weight"));

	return EvaluateRactangle(x_1, y_1, x_2, y_2, weight);
}