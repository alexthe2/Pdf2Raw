#pragma once
#include <opencv2/opencv.hpp>
#include <boost/log/trivial.hpp>

typedef std::tuple<int, int, int, int> RegionOfInterest;
#define ROI RegionOfInterest

/// <summary>
/// Crop an Image <br />
/// <br />
/// <b>Depends on:</b>											  <br />
///	<see href="https://opencv.org/">opencv</see> for cropping <br />
///	<see href="https://www.boost.org/">Boost</see> for logging	  <br />
/// </summary>
class ImageSelection {
public:
	/// <summary>
	/// Create and load an image
	/// </summary>
	/// <param name="path">The path to the image, local or global </param>
	explicit ImageSelection(const std::string& path);
	~ImageSelection();

	/// <summary>
	/// Tries to crop the image with the given region
	/// </summary>
	/// <param name="region">A tuple with x1,x2,y1,y2</param>
	/// <returns>Whether the cropping was successful</returns>
	auto Convert(ROI region) -> bool;

	/// <summary>
	/// Try to save the result
	/// </summary>
	/// <param name="overwrite">Whether the current image should be overwritten or a new image should be made</param>
	auto Save(bool overwrite = false) -> void;
	
	/// <summary>
	/// Try to save the result
	/// </summary>
	/// <param name="path">The path for the new image</param>
	auto Save(const std::string& path) const -> void;

	/// <summary>
	/// Returns a copy of the current selection
	/// </summary>
	/// <returns>A copy of the current selection</returns>
	[[nodiscard]] auto GetImageCopy() const -> cv::Mat;
	
private:
	auto Build() -> void;
	auto SaveImage(const std::string& path) const -> void;
	
	std::string path_;
	cv::Mat img_;
};

