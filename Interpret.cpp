#include "Interpret.h"
Interpret::Interpret(const cv::Mat img, const std::string tess_location, std::string language) {
	this->img_ = img;
	this->internally_created_ = false;
	this->tess_location_ = tess_location;
	this->lang_ = language;

	this->InitAndEvaluate();
}

Interpret::Interpret(const ImageSelection is, const std::string tess_location, std::string language) {
	this->img_ = is.GetImageCopy();
	this->internally_created_ = true;
	this->tess_location_ = tess_location;
	this->lang_ = language;
	
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

	BOOST_LOG_TRIVIAL(info) << "Setting language to " << this->lang_;
	this->ocr_->Init(this->tess_location_.c_str(), this->lang_.c_str(), tesseract::OEM_LSTM_ONLY);
	this->ocr_->SetPageSegMode(tesseract::PSM_AUTO);

	this->ocr_->SetImage(this->img_.data, this->img_.cols, this->img_.rows, 3, this->img_.step);
	this->text_ = std::string(this->ocr_->GetUTF8Text());
	BOOST_LOG_TRIVIAL(info) << "Image has been evaluated";
}
