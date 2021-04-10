// ReSharper disable once CommentTypo
// ReSharper disable CppClangTidyBugproneExceptionEscape
#include "PDF2Image.h"

Pdf2Image::Pdf2Image(const std::string path): path_(path) {
	this->Build();
}

Pdf2Image::~Pdf2Image() {
	this->Release();
}

auto Pdf2Image::Check() const -> bool {
	return this->good_;
}

auto Pdf2Image::Size() const -> int {
	return this->Size();
}

auto Pdf2Image::Render(const std::string &export_name, const int page, const int zoom) -> bool {
	if(!this->good_) {
		return false;
	}

	this->export_name_ = export_name;
	this->GenerateImageName();
	
	auto ctm = fz_scale(zoom / 100.0f, zoom / 100.0f);
	ctm = fz_pre_rotate(ctm, 0);
	
	fz_try(this->ctx_) {
		/* Try to convert the given page*/
		auto *pix = fz_new_pixmap_from_page_number(this->ctx_, this->doc_, page - 1, ctm, fz_device_rgb(this->ctx_), 0);
		fz_save_pixmap_as_png(this->ctx_, pix, this->export_name_.c_str());
		BOOST_LOG_TRIVIAL(info) << "Successfully converted and saved";
	} fz_catch(this->ctx_) {
		BOOST_LOG_TRIVIAL(error) << "Failed to convert Pdf to image";
		return false;
	}

	return true;
}

auto Pdf2Image::GenerateImageName() -> void {
	const auto filename = this->export_name_.append(".png");
	this->image_name_ = filename;

	BOOST_LOG_TRIVIAL(info) << "Image name will be: " << filename;
}

auto Pdf2Image::Build() -> void {
	BOOST_LOG_TRIVIAL(info) << "Loading PDF " << this->path_;

	this->good_ = true;
	this->image_name_ = "";

	this->ctx_ = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	if (!this->ctx_) {
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't create context!";
		this->good_ = false;
		return;
	}

	fz_try(this->ctx_) {
		/* Load pdf */
		fz_register_document_handlers(this->ctx_);
		this->doc_ = fz_open_document(this->ctx_, this->path_.c_str());
		this->size_ = fz_count_pages(this->ctx_, this->doc_);

		BOOST_LOG_TRIVIAL(info) << "Successfully read " << this->size_ << " pages";
	} fz_catch(this->ctx_) {
		BOOST_LOG_TRIVIAL(fatal) << "Couldn't read pdf";
		this->doc_ = nullptr;
		this->good_ = false;
	}
}

auto Pdf2Image::Release() const -> void {
	fz_try(this->ctx_) {
		if (this->doc_) {
			fz_drop_document(this->ctx_, this->doc_);
			BOOST_LOG_TRIVIAL(info) << "Cleaned up";
		}
	} fz_catch(this->ctx_) {
		BOOST_LOG_TRIVIAL(error) << "Couldn't clean up";
	}

	fz_drop_context(this->ctx_);
}

