#pragma once
#include <string>
#include <boost/log/trivial.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

#include "ImageSelection.h"

#define TESSDATA_LOCATION "H:\\tessdata"

/// <summary>
/// OCR class<br />
/// <br />
/// <b>Depends on:</b>											  <br />
///	<see href="https://www.boost.org/">Boost</see> for logging	  <br />
///	<see href="https://www.opencv.org/">opencv</see> for the image <br />
///	<see href="http://www.leptonica.org/">Leptonica</see> for tesseract <br />
///	<see href="https://github.com/tesseract-ocr/tesseract">Tesseract</see> for the OCR <br />
/// </summary>
class Interpret {
public:
	/// <summary>
	/// Evaluate the OCR based on an image
	/// </summary>
	/// <param name="img">The image</param>
	explicit Interpret(cv::Mat img);

	/// <summary>
	/// Evaluate the OCR based on an ImageSelection, by copying an image, will be freed on release
	/// </summary>
	/// <param name="is"></param>
	explicit Interpret(ImageSelection is);
	~Interpret();

	/// <summary>
	/// Get the result
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] auto GetText() const -> std::string;

private:
	std::string text_;
	
	bool internally_created_;
	cv::Mat img_;
	tesseract::TessBaseAPI* ocr_ = nullptr;

	auto InitAndEvaluate() -> void;
};

