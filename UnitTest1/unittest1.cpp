#include "stdafx.h"
#include "CppUnitTest.h"
#include <Fogo.h>

#pragma comment(lib, "libfbxsdk-md.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using Fogo::Game::Component;
using Fogo::Game::Store;
using Fogo::Utility::Result;

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
				Result<Error, int> operator()(int number) {
					if (number > 0) return number;
					return Error::SampleError;
				}
			};

			Assert::IsTrue(testFunc{}(1));
		}
	};

	TEST_CLASS(ComponentTest1) {
	public:
		struct Component1 : Component {
			struct Meta : Component {
				int age;
				std::string name;
				Meta(int age_, std::string name_) : age(age_), name(name_) {}
			};

			struct Position : Component {
				float x, y, z;
				Position(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
			};

			Component1(int age, std::string name) {
				create<Meta>(age, name);
				create<Position>(age, age, age);
			}
		};

		struct Master : Component {
			Master() {
				create<Component1>(10, "Foo");
			}
		};

		TEST_METHOD(method1) {
			Master master;
			const auto & foo = master.get<Component1>();
			Assert::IsTrue(foo);

			const auto & meta = foo->get<Component1::Meta>();
			Assert::IsTrue(foo);

			const auto & position = foo->get<Component1::Position>();
			Assert::IsTrue(position);

			Assert::AreEqual(10.0f, position->x);
		}
	};
}