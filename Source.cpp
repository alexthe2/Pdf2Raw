#include <argparse/argparse.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <iostream>
#include <cstdio>
#include <fstream>

#include "PDF2Image.h"
#include "ImageSelection.h"
#include "Interpret.h"


int main(int argc, char **argv) {
	argparse::ArgumentParser program("Tribo2Raw", "1.0");

	program.add_argument("-f", "--file").required().help("The Input file");
	program.add_argument("-o", "--out").required().help("The Output file");
	program.add_argument("--nolog").help("Then only errors and fatal will be shown").default_value(false).implicit_value(true);
	program.add_argument("--page").help("The page which should be taken").default_value(std::string("1"));
	program.add_argument("--zoom").help("Zoom factor for OCR").default_value(std::string("100"));
	program.add_argument("--cord").help("The absolute coordinates which should be taken, on processing").nargs(4);
	program.add_argument("--temp-file").help("The name of the temporary file").default_value(std::string("conversion.temp"));

	try {
		program.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		BOOST_LOG_TRIVIAL(fatal) << err.what();
		BOOST_LOG_TRIVIAL(fatal) << program;
		return EXIT_FAILURE;
	}

	if (program["--nolog"] == true) {
		boost::log::core::get()->set_filter(
			boost::log::trivial::severity >= boost::log::trivial::error
		);
	}

	auto input = program.get<std::string>("-f");
	auto output = program.get<std::string>("-o");
	auto temp = program.get<std::string>("--temp-file");
	
	auto page = std::stoi(program.get<std::string>("--page"));
	auto zoom = std::stoi(program.get<std::string>("--zoom"));
	
	Pdf2Image pdf_img(input);
	if(!pdf_img.Render(temp, page, zoom)) {
		return EXIT_FAILURE;
	}
	
	ImageSelection img_sel(temp.append(".png"));
	if(program.is_used("--cord")) {
		auto points = program.get<std::vector<std::string>>("--cord");
		auto conv = img_sel.Convert(std::tuple<int, int, int, int>
			(std::stoi(points.at(0)), std::stoi(points.at(1)), std::stoi(points.at(2)), std::stoi(points.at(3))));

		if(!conv) {
			BOOST_LOG_TRIVIAL(error) << "Conversion failed, full image will be taken!";
		}
	}
	
	Interpret interpret(img_sel);

	std::ofstream out(output, std::ios::out);
	out << interpret.GetText();
	out.close();

	remove(temp.append(".png").c_str());
	return EXIT_SUCCESS;
}