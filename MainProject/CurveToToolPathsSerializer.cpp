#include "CurveToToolPathsSerializer.h"
#include <fstream>
#include <iomanip>
#include <sstream>

bool CurveToToolPathsSerializer::Serialize(std::vector<std::shared_ptr<Point>> points, std::filesystem::path filePath)
{
	std::ofstream out(filePath, std::ios_base::app);


	int lineNumber = 2;
	std::string lastX = "";
	std::string lastY = "";
	std::string lastZ = "";
	for (auto point : points)
	{
		auto translation = point->GetTranslation();
		std::stringstream stream;
		std::stringstream Xstream;
		std::stringstream Ystream;
		std::stringstream Zstream;

		Xstream << std::fixed << std::setprecision(3) << "X" << translation.x * 10;
		Ystream << std::fixed << std::setprecision(3) << "Y" << translation.z * 10;
		Zstream << std::fixed << std::setprecision(3) << "Z" << translation.y * 10;
		auto xString = Xstream.str();
		auto yString = Ystream.str();
		auto zString = Zstream.str();
		if (lastX == xString && lastY == yString && lastZ == zString)
		{
			continue;
		}
		stream << "N" << ++lineNumber << "G01";
		if (lastX != xString)
		{
			lastX = xString;
			stream << xString;
		}
		if (lastY != yString)
		{
			lastY = yString;
			stream << yString;
		}
		if (lastZ != zString)
		{
			lastZ = zString;
			stream << zString;
		}

		auto lineString = stream.str();

		out << lineString << "\n";
	}




	out.close();


	return true;
}
