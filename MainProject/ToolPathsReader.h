#pragma once
#include "ToolPaths.h"
#include <memory>
#include <string>

class ToolPathsReader
{
public:
	static std::shared_ptr<ToolPaths> ReadFromFile(std::string filePath);

	static std::string GetCommand(std::string line, int& commandLineNumber, int lineNumber);

private:
};