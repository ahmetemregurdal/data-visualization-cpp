#include <argparse/argparse.hpp>
#include "hedley.h"
#include "csv.hpp"
#include <string>
#include <iostream>
#include <exception>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <matplot/matplot.h>

std::string inputFile;
char seperatorChar;
std::string xAxisLabel;
std::string dateLabel;
std::string classLabel;

auto parseArgs(int argc, char **argv) -> void
{
	argparse::ArgumentParser parser("cpp-data-visualiser");
	parser.add_argument("inputFile")
		.help("File used in the prgogram as input");

	parser.add_argument("xAxisLabel")
		.help("Label for the x-axis");

	parser.add_argument("dateLabel")
		.help("Label for the date column");

	parser.add_argument("classLabel")
		.help("Label for the different classess of the data points");

	parser.add_argument("-s", "--seperator")
		.help("Seperator character in csv, default value is ','")
		.default_value(std::string(","));

	try
	{
		parser.parse_args(argc, argv);
	}
	catch (const std::exception &err)
	{ // unable to parse arguments
		std::cerr << err.what() << std::endl;
		std::cerr << parser << std::endl;
		exit(1);
	}
	inputFile = parser.get("inputFile"); // set csv input file
	xAxisLabel = parser.get("xAxisLabel");
	dateLabel = parser.get("dateLabel");
	classLabel = parser.get("classLabel");
	if (parser.get<std::string>("--seperator").size() != 1)
	{
		throw std::domain_error("--seperator must have a single character value");
	}
	seperatorChar = parser.get<std::string>("--seperator").at(0);
}

int parseDate(const std::string &date)
{
	std::string year = date.substr(0, 4);
	std::string month = date.substr(5, 2);
	std::string day = date.substr(8, 2);
	return std::stoi(year) * 10000 + std::stoi(month) * 100 + std::stoi(day);
}

int main(int argc, char **argv)
{
	parseArgs(argc, argv);
	csv::CSVFormat format;
	format.delimiter(seperatorChar)
		.header_row(0)
		.quote('"');
	csv::CSVReader doc(inputFile, format);
	std::vector<std::string> classLabelInOrder;
	std::map<std::string, std::vector<int>> dataMap;
	std::vector<std::string> dateColumn;
	std::vector<int> xAxisColumn;
	for (auto &row : doc)
	{
		classLabelInOrder.push_back(row[classLabel].get<>());
		dateColumn.push_back(row[dateLabel].get<>());
		if (!row[xAxisLabel].is_int())
		{
			throw std::domain_error("Data column must be integer");
		}
		xAxisColumn.push_back(row[xAxisLabel].get<int>());
	}
	std::vector<std::string> classessUnique = classLabelInOrder;
	sort(classessUnique.begin(), classessUnique.end());
	{
		auto last = std::unique(classessUnique.begin(), classessUnique.end());
		classessUnique.erase(last, classessUnique.end());
	}
	for (int i = 0; i < classLabelInOrder.size(); i++)
	{
		dataMap[classLabelInOrder[i]].push_back(xAxisColumn[i]);
	}
	matplot::title("Data Visualisation");
	matplot::xlabel(xAxisLabel);
	matplot::ylabel(dateLabel);
	std::vector<std::vector<int>> prePlot(dataMap[classessUnique[0]].size(), std::vector<int>());
	std::cerr << "97\n";
	for (auto x : classessUnique)
	{
		for (int i = 0; i < dataMap[x].size(); i++)
		{
			prePlot[i].push_back(dataMap[x][i]);
			std::cerr << "101: " << i << '\n';
		}
	}
	std::cerr << "104\n";
	matplot::plot(prePlot);
	matplot::xticklabels(dateColumn);
	matplot::legend(classessUnique);
	matplot::show();
}