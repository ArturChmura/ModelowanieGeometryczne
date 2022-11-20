#include "CurveToToolPathsSerializer.h"
#include <fstream>
#include <iomanip>
#include <sstream>

bool CurveToToolPathsSerializer::Serialize(std::vector<std::shared_ptr<Point>> points, std::filesystem::path filePath)
{
    std::ofstream out(filePath, std::ios_base::app);


    int lineNumber = 2;
    for (auto point : points)
    {
        std::stringstream stream;
        auto translation = point->GetTranslation();
        stream << "N" << ++lineNumber << "G01"
         << std::fixed << std::setprecision(3)
            << "X" << translation.x*10
            << "Y" << translation.z * 10
            << "Z" << translation.y * 10;
        auto lineString = stream.str();

        out << lineString << "\n";
    }




    out.close();


    return true;
}
