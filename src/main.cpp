#include "../dep/rapidcsv/src/rapidcsv.h"
#include "../dep/argparse/include/argparse/argparse.hpp"
#include "../dep/hedley/hedley.h"
#include "string"
#include "iostream"
#include "exception"
#include "vector"
#include "algorithm"
#include "stdexcept"

std::string inputFile;
char seperatorChar;

auto parseArgs(int argc, char** argv) -> void {
	argparse::ArgumentParser parser("cpp-data-visualiser");
	parser.add_argument("inputFile")
		.help("File used in the prgogram as input");

	parser.add_argument("-s", "--seperator")
		.help("Seperator character in csv, default value is ';'")
		.default_value(std::string(";"));

	try {
		parser.parse_args(argc, argv);
	}
	catch (const std::exception& err) { // unable to parse arguments
		std::cerr << err.what() << std::endl;
		std::cerr << parser << std::endl;
		exit(1);
	}
	inputFile = parser.get("inputFile"); // set csv input file
	if(parser.get<std::string>("--seperator").size() != 1) {
		std::cerr << "Seperator should be a single character" << std::endl;
		exit(1);
	}
	seperatorChar = parser.get<std::string>("--seperator").at(0);
}

int main(int argc, char** argv) {
	parseArgs(argc, argv);
	rapidcsv::Document doc(inputFile, rapidcsv::LabelParams(0, 0),
						rapidcsv::SeparatorParams(seperatorChar));
	std::vector<std::string> columnNames = doc.GetColumnNames();
}
