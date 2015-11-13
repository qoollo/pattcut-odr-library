cmake_minimum_required (VERSION 2.8)
project (RecognitionLib)

# ========== Файлы исходного кода ==========

set(LIBS_SOURCE_FILES)
set(LIBS_HEADERS)

set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/export/test_export.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/export/pattcut/c_api/Common.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/export/pattcut/c_api/Image.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/export/pattcut/c_api/RecognitionFunctions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/export/pattcut/cpp_api/RecognitionController.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/export/pattcut/cpp_api/Image.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/images/ImageLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/images/IntegralImage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/images/WindowIntegralImage.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/tests/test_integral_image.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/haar_features/EvaluateRactangle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/haar_features/HaarFeature.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/haar_features/EvaluateRactangleCompact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/haar_features/HaarFeatureCompact.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/simple/StumpClassifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/simple/StumpClassifierCompact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/simple/StatMomentsClassifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/boosted/BoostedClassifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/boosted/BoostedClassifierCompact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/cascade/CascadeClassifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/cascade/CascadeClassifierCompact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/viola_jones/ViolaJonesClassifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifiers/multi_class_viola_jones/MultiClassViolaJonesController.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/post_filtration/StandardResultFiltration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/post_filtration/ResDataCombination.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/TinyXML/tinystr.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/TinyXML/tinyxml.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/TinyXML/tinyxmlerror.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/TinyXML/tinyxmlparser.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/classifier_loaders/ClassifierLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifier_loaders/ClassifierLoaderPattCut.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/classifier_loaders/ClassifierLoaderOpenCV.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/src/accuracy_estimation/SimpleAccuracyEstimator.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/src/accuracy_estimation/LogisticAccuracyEstimator.cpp	
)

set(EXPORT_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/export_helper.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/PattCutCPP.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/PattCutC.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/c_api/Common.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/c_api/Image.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/c_api/RecognitionResults.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/c_api/RecognitionFunctions.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/cpp_api/RecognitionResults.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/cpp_api/RecognitionController.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/cpp_api/Image.h
)

set(HEADER_FILES
	${EXPORT_HEADER_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/inc/export/test_export.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/support/Exceptions.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/support/OwnTrackingPtr.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/support/RectangleF.h	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/support/Rectangle.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/support/PlatformHelperMacro.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/images/MatrixStorage.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/images/Image.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/images/ImageLoader.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/images/IntegralImage.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/images/WindowData.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/images/WindowIntegralImage.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/tests/test_integral_image.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/haar_features/EvaluateRactangle.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/haar_features/HaarFeature.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/haar_features/EvaluateRactangleCompact.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/haar_features/HaarFeatureCompact.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/ClassificationResult.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/ClassificationTag.h

	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/simple/StatMomentsClassifier.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/simple/FixedAreaStatMomentsClassifier
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/simple/StumpClassifier.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/simple/StumpClassifierCompact.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/boosted/BoostedClassifier.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/boosted/BoostedClassifierCompact.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/cascade/CascadeClassifier.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/cascade/CascadeClassifierCompact.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/cascade/CascadeStatistic.h

	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/viola_jones/ViolaJonesClassifier.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/viola_jones/ScaledCascadeInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/viola_jones/ScaledCascadeInfoCompact.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/viola_jones/AreaWithExtInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/viola_jones/ViolaJonesOutputWithStatistic.h

	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesController.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesRecognitionResult.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/multi_class_viola_jones/CapturedAreaInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/multi_class_viola_jones/ResultByClassifier.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifiers/multi_class_viola_jones/MultiClassViolaJonesRecognitionDataInfo.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/post_filtration/FilteredAreaInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/post_filtration/FilteredAreaWithExtInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/post_filtration/StandardResultFiltration.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/post_filtration/ResDataCombination.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/post_filtration/ViolaJonesFilteredOutputWithStatistic.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/TinyXML/tinystr.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/TinyXML/tinyxml.h

	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifier_loaders/ClassifierLoader.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifier_loaders/ClassifierLoaderPattCut.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/classifier_loaders/ClassifierLoaderOpenCV.h

	${CMAKE_CURRENT_SOURCE_DIR}/inc/accuracy_estimation/AccuracyResults.h	
	${CMAKE_CURRENT_SOURCE_DIR}/inc/accuracy_estimation/SimpleAccuracyEstimator.h
	${CMAKE_CURRENT_SOURCE_DIR}/inc/accuracy_estimation/LogisticAccuracyEstimator.h
)

set(MODULE_DEFINITION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/export/RecognitionLib.def)



# ============ Библиотеки ================


#if(WIN32)
#	find_package(ImageMagick COMPONENTS Magick++ REQUIRED CONFIGS FindImageMagickCustomWin.cmake PATHS ${CMAKE_MODULE_PATH})
#else(WIN32)
#	find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
#endif(WIN32)


find_package(LibJpegTurbo REQUIRED)
find_package(PNG REQUIRED)


if(MSVC)
	# Помогаем Visual Studio отсортировать файлы
	macro(GroupSources curdir)
	   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
	   foreach(child ${children})
			  if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
					GroupSources(${curdir}/${child})		  
			  else()
					string(REPLACE "/" "\\" groupname ${curdir})
					source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})				
			  endif()
	   endforeach()
	endmacro()

	GroupSources("src")
	GroupSources("inc")
	
	set_source_files_properties(${MODULE_DEFINITION_FILE} PROPERTIES HEADER_FILE_ONLY TRUE)
endif(MSVC)


# ========== include директории ============

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/export/)
#include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${LibJpegTurbo_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})

# ========= Таргет с настройками ===============

add_library(RecognitionLib SHARED ${SOURCE_FILES} ${HEADER_FILES} ${MODULE_DEFINITION_FILE})
set_target_properties(RecognitionLib PROPERTIES OUTPUT_NAME "PattCutLib")
if(MSVC)
	set_target_properties(RecognitionLib PROPERTIES LINK_FLAGS -DEF:"${MODULE_DEFINITION_FILE}")
	set_target_properties(RecognitionLib PROPERTIES LINK_FLAGS /LTCG)
endif(MSVC)

#target_link_libraries(RecognitionLib ${ImageMagick_LIBRARIES})
target_link_libraries(RecognitionLib ${LibJpegTurbo_LIBRARY})
target_link_libraries(RecognitionLib ${PNG_LIBRARIES})


add_custom_target(build_lib DEPENDS RecognitionLib)
set_property(TARGET build_lib PROPERTY FOLDER "Helpers")

# ============= Параметры install ==============

install(TARGETS RecognitionLib DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/export/pattcut/ DESTINATION inc/pattcut)
