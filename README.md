## PattCut Object Detection and Recognition library (http://pattcut.com/)

**Main features:**

* Small, simple and fast
* Can detect multiple objects at one pass
* Returns extended information about detected areas
* Works with PattCut classifiers and OpenCV cascade classifiers
* Support JPEG, PNG and BMP image formats


**Usage example:**

```C++
PattCutLib::RecognitionController controller;
auto classifier_id = controller.load_classifier_from_file("classifier.xml");
auto res = controller.perform_recognition("image.jpg");
for (int i = 0; i < res.result_per_classifier(0).areas().size(); i++)
{
	auto cur_area = res.result_per_classifier(0).areas(i);
	std::cout << "veracity: " << cur_area.veracity() <<
				", wnd_count: " << cur_area.merged_window_count() <<
				", area: [" << cur_area.rect_x() << ", " << cur_area.rect_y() << "; " << cur_area.rect_width() << ", " << cur_area.rect_height() << "]" << std::endl;
}
```


**Extended usage example:**

https://github.com/qoollo/pattcut-odr-library/blob/master/src/RecognitionLibTest/src/main.cpp


**Other information:**

* [How to build](https://github.com/qoollo/pattcut-odr-library/wiki/How-to-build)
* [How to use](https://github.com/qoollo/pattcut-odr-library/wiki/How-to-use)
* [Comparison with OpenCV](https://github.com/qoollo/pattcut-odr-library/wiki/Comparison-with-OpenCV)
* [Third-party libraries](https://github.com/qoollo/pattcut-odr-library/wiki/Third-party-libraries)
