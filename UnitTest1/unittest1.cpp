#include "stdafx.h"
#include "CppUnitTest.h"
#include <Fogo.h>

#pragma comment(lib, "libfbxsdk-md.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fogo;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			enum class Error {
				SampleError
			};

			struct testFunc {
				Utility::Result<Error, int> operator()(int number) {
					if (number > 0) return number;
					return Error::SampleError;
				}
			};

			Assert::IsTrue(testFunc{}(1));
		}

	};
}