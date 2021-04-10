#include "ImageSelection.h"
ImageSelection::ImageSelection(const std::string& path) : path_(path) {
	this->Build();
}

ImageSelection::~ImageSelection() {
	this->img_.release();
}

auto ImageSelection::Convert(ROI region) -> bool {
	const auto x1 = std::get<0>(region);
	const auto y1 = std::get<1>(region);
	const auto x2 = std::get<2>(region);
	const auto y2 = std::get<3>(region);

	if (x2 < x1 || y2 < y1) {
		BOOST_LOG_TRIVIAL(error) << "Negative rectangle, Not yet implemented";
		return false;
	}

	if (x1 < 0 || y1 < 0 ) {
		BOOST_LOG_TRIVIAL(error) << "Cannot select region from x|y lower than 0";
		return false;
	}

	if (this->img_.cols <= x2 || this->img_.rows < y2) {
		BOOST_LOG_TRIVIAL(error) << "Cannot select region from x|y that is out of bounds";
		return false;
	}

	const auto cv_roi = cv::Rect(x1, y1, x2 - x1, y2 - y1);
	this->img_ = this->img_(cv_roi);

	BOOST_LOG_TRIVIAL(info) << "Successfully shrink to " << cv_roi.size().height << "x" << cv_roi.size().width;
	return true;
}

auto ImageSelection::Save(const bool overwrite) -> void {
	const auto path = overwrite ? this->path_ : this->path_.append(".selection.png");
	this->SaveImage(path);
}

auto ImageSelection::Save(const std::string& path) const -> void {
	this->SaveImage(path);
}

auto ImageSelection::GetImageCopy() const -> cv::Mat {
	return img_.clone();
}

auto ImageSelection::Build() -> void {
	BOOST_LOG_TRIVIAL(info) << "Loading picture from: " << this->path_;

	this->img_ = cv::imread(this->path_, cv::IMREAD_UNCHANGED);
	if(this->img_.empty()) {
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't read image!";
	}
}

auto ImageSelection::SaveImage(const std::string& path) const -> void {
	BOOST_LOG_TRIVIAL(info) << "Saving image under: " << path;
	cv::imwrite(path, this->img_);
}
