#include "ToolPathsReader.h"
#include <fstream>
#include "StringExtensions.h"
#include <regex>
#include <iostream>

std::shared_ptr<ToolPaths> ToolPathsReader::ReadFromFile(std::string filePath)
{
	std::ifstream input;
	input.open(filePath);
	int lineNumber = 1;
	int commandLineNumber = -1;
	std::string line;

	if (std::getline(input, line))	 //pierwsza linia zawiera sekwencje informuj¹ca maszynê o stosowanych w programie jednostkach metrycznych : w przypadku milimetrów jest to : % G71(np.dla cali jest to % G70),
	{
		trim(line);
		if (line == "%G71" || line == "%G70")
		{
			lineNumber++;
			// jednostki, ale tego nie obs³ugujemy
		}
		else
		{
			input.clear();
			input.seekg(0, std::ios::beg);
			commandLineNumber = -1;
		}
	}

	std::streampos oldpos = input.tellg();  // stores the position
	if (std::getline(input, line))		 // pierwsza instrukcja zawiera sekwencje pocz¹tkow¹ : N1G40G90 informuj¹c¹ maszynê, ¿e uk³ad wspó³rzêdnych jest globalny a nie lokalny 	(wed³ug œrodka narzêdzia),
	{
		auto command = GetCommand(line, commandLineNumber, lineNumber);
		if (command == "G40G90")
		{
			lineNumber++;
			// ignorujemy
		}
		else
		{
			input.seekg(oldpos);
			commandLineNumber = -1;
		}
	}


	std::vector<std::tuple<double, double, double>> positions;
	for (std::string line; std::getline(input, line); )
	{
		lineNumber++;
		auto command = GetCommand(line, commandLineNumber, lineNumber);

		double X, Y, Z;

		if (command == "M05" || command == "M30")
		{
			break;
		}
		else if (begins(command, "G"))
		{
			auto mode = command.substr(1, 2);
			if (mode == "00" || mode == "01")
			{
				std::smatch sm;
				std::regex x_expr("X-?\\d+\\.\\d{3}");
				std::regex y_expr("Y-?\\d+\\.\\d{3}");
				std::regex z_expr("Z-?\\d+\\.\\d{3}");

				if (regex_search(command, sm, x_expr))
				{
					std::string xCommand = sm[0];
					std::string xString = xCommand.substr(1);
					X = std::stod(xString);
				}
				if (regex_search(command, sm, y_expr))
				{
					std::string yCommand = sm[0];
					std::string yString = yCommand.substr(1);
					Y = std::stod(yString);
				}
				if (regex_search(command, sm, z_expr))
				{
					std::string zCommand = sm[0];
					std::string zString = zCommand.substr(1);
					Z = std::stod(zString);
				}

				auto position = std::make_tuple(X, Z, Y); // zmiana, bo inny uk³ad wsp
				positions.push_back(position);
			}
			else
			{
				throw  std::exception(("Error on line " + std::to_string(lineNumber) + ". Unrecognized command" + command).c_str());
			}
		}
		else
		{
			throw  std::exception(("Error on line " + std::to_string(lineNumber) + ". Unrecognized command" + command).c_str());
		}

	}

	auto toolPaths = std::make_shared<ToolPaths>(positions);

	return toolPaths;
}

std::string ToolPathsReader::GetCommand(std::string line, int& commandLineNumber, int lineNumber)
{
	trim(line);

	std::smatch sm;
	std::regex str_expr("N\\d+");

	if (!regex_search(line, sm, str_expr))
	{
		throw std::exception(("Error on line " + std::to_string(lineNumber) + ". Command does not starts with line number").c_str());
	}
	std::string numberInfo = sm[0];
	std::string numberString = numberInfo.substr(1);

	int number = std::stoi(numberString);

	if (number - 1 != commandLineNumber && commandLineNumber >= 0)
	{
		throw std::exception(("Error on line " + std::to_string(lineNumber) + ". Command number did not incresed by 1").c_str());
	}

	commandLineNumber = number;


	std::string command = line.substr(numberInfo.size());

	return command;

}

