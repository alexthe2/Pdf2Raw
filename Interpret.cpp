#include "Interpret.h"
Interpret::Interpret(const cv::Mat img) {
	this->img_ = img;
	this->internally_created_ = false;

	this->InitAndEvaluate();
}

Interpret::Interpret(const ImageSelection is) {
	this->img_ = is.GetImageCopy();
	this->internally_created_ = true;

	this->InitAndEvaluate();
}

Interpret::~Interpret() {
	if(this->internally_created_) {
		this->img_.release();
	}

	this->ocr_->End();
}

auto Interpret::GetText() const -> std::string {
	return this->text_;
}

auto Interpret::InitAndEvaluate() -> void {
	BOOST_LOG_TRIVIAL(info) << "Initializing Tesseract";
	this->ocr_ = new tesseract::TessBaseAPI();

	BOOST_LOG_TRIVIAL(info) << "Setting language to english";
	this->ocr_->Init(TESSDATA_LOCATION, "eng", tesseract::OEM_LSTM_ONLY);
	this->ocr_->SetPageSegMode(tesseract::PSM_AUTO);

	this->ocr_->SetImage(this->img_.data, this->img_.cols, this->img_.rows, 3, this->img_.step);
	this->text_ = std::string(this->ocr_->GetUTF8Text());
	BOOST_LOG_TRIVIAL(info) << "Image has been evaluated";
}
