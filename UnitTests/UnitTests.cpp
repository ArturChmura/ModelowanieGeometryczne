#include "pch.h"
#include "CppUnitTest.h"
#include <d3d11.h>
#include "../MainProject/IParameterized.h"
#include "../MainProject/IntersectionFinder.h"
#include "../MainProject/IntersectionFinder.cpp"
#include "ParametrizedMocks.h"
#include "../MainProject/Torus.h"

#define PI 3.14159265358979323846 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX::SimpleMath;
namespace UnitTests
{
	



	TEST_CLASS(UnitTests)
	{
	public:

		TEST_METHOD(IntersectionTest)
		{
			IntersectionFinder intersectionFinder;
			auto p1 = std::make_shared< ParametrizedMock21>();
			auto p2 = std::make_shared< ParametrizedMock22>();
			intersectionFinder.FindNearestPoint(p1, p2,
				-1,1,
				-2,8,
				0,3,
				0,3
				);
		}
	};
}
