#include "pch.h"
#include "CppUnitTest.h"
#include <d3d11.h>
#include "../MainProject/IParameterized.h"
#include "../MainProject/IntersectionFinder.h"
#include "../MainProject/IntersectionFinder.cpp"

#define PI 3.14159265358979323846 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX::SimpleMath;
#include "ParametrizedMocks.h"
namespace UnitTests
{
	



	TEST_CLASS(UnitTests)
	{
	public:

		TEST_METHOD(IntersectionTest)
		{
			IntersectionFinder intersectionFinder;
			auto p1 = std::make_shared< ParametrizedMock1>();
			auto p2 = std::make_shared< ParametrizedMock2>();
			intersectionFinder.FindIntersection(p1, p2,
				0, 1,
				0, 1,
				- PI/4, 3 * PI/4,
				0, 2*PI
				);
		}
	};
}
