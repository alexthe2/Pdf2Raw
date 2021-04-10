// ReSharper disable CppClangTidyBugproneExceptionEscape
#pragma once

extern "C" {
	#include <mupdf/fitz.h>
}

#include <boost/log/trivial.hpp>
#include <string>

/// <summary>
/// Creating an Image from a PDF <br />
/// <br />
/// <b>Depends on:</b>											  <br />
///	<see href="https://www.mupdf.com">MuPDF</see> for conversion  <br />
///	<see href="https://www.boost.org/">Boost</see> for logging	  <br />
/// </summary>
class Pdf2Image {
public:
	/// <summary>
	/// Load a PDF
	/// </summary>
	/// <param name="path">local or global path</param>
	explicit Pdf2Image(std::string path);
	~Pdf2Image();
	
	/// <summary>
	/// Check whether loading was successful
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] auto Check() const -> bool;

	/// <summary>
	/// Get amount of pages the pdf has
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] auto Size() const -> int;

	/// <summary>
	/// Try to convert the given page to a pdf
	/// </summary>
	/// <param name="export_name">The filename without an extension</param>
	/// <param name="page">The page which should be converted</param>
	/// <param name="zoom">The zoom parameter that should be used</param>
	/// <returns>Success or Failure</returns>
	[[nodiscard]] auto Render(const std::string& export_name, int page, int zoom) -> bool;
	
private:
	fz_context* ctx_{};
	fz_document* doc_{};

	bool good_{};
	int size_{};
	
	std::string image_name_;
	std::string path_;
	std::string export_name_;

	auto GenerateImageName() -> void;
	auto Build() -> void;
	auto Release() const -> void;
};

