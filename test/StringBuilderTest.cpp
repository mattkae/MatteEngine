#include "CppUnitTest.h"
#include "../src/List.h"
#include "../src/MyString.h"
#include "../src/MyString.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Primitives {
	const char* LONG_STR = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";
	TEST_CLASS(StringBuilderTest)
	{
		public:
			StringBuilder sb;
			String output;
			TEST_METHOD_CLEANUP(FREE)
			{
				sb.free();
				output.free();
			}

			TEST_METHOD(Simple)
			{
				sb.addStr("Hello World!");
				String output = sb.toString();
				char* outValue = output.getValue();
				Assert::AreEqual("Hello World!", outValue);
			}
			
			TEST_METHOD(VeryLong)
			{
				StringBuilder sb;
				sb.addStr(LONG_STR);
				String output = sb.toString();
				char* outValue = output.getValue();
				Assert::AreEqual(LONG_STR, outValue);
			}

			TEST_METHOD(Simple_IntegerTest)
			{
				const int x = 5;
				sb.addInt(x);
				output = sb.toString();
				Assert::AreEqual("5", output.getValue());
			}

			TEST_METHOD(Long_IntegerTest)
			{
				const int x = 8302;
				sb.addInt(x);
				output = sb.toString();
				Assert::AreEqual("8302", output.getValue());
			}

			TEST_METHOD(Simple_FloatTest)
			{
				const float x = 3.33;
				sb.addFloat(x);
				output = sb.toString();
			}
				
			TEST_METHOD(Long_FloatTest)
			{
				const float x = 2321.421f;
				sb.addFloat(x);
				output = sb.toString();
				Assert::AreEqual("2321.421", output.getValue());
			}

			TEST_METHOD(FORMAT_1)
			{
				sb.format("Hello %d", (int)5);
				output = sb.toString();
				Assert::AreEqual("Hello 5", output.getValue());
			}

			TEST_METHOD(FORMAT_2)
			{
				sb.format("Hello %d %f", (int)5, 3.33f);
				output = sb.toString();
				Assert::AreEqual("Hello 5 3.33", output.getValue());
			}

			TEST_METHOD(FORMAT_3)
			{
				sb.format("Hello %s", "WORLD");
				output = sb.toString();
				Assert::AreEqual("Hello WORLD", output.getValue());
			}


			TEST_METHOD(CLEAR)
			{
				sb.format("Numerical %d", 10);
				output = sb.toString();
				Assert::AreEqual("Numerical 10", output.getValue());
				sb.clear();
				sb.format("Two %d", 2);
				output = sb.toString();
				Assert::AreEqual("Two 2", output.getValue());
			}

			TEST_METHOD(REMOVE_AT_LONG)
			{
				sb.addStr(LONG_STR);
				sb.removeAt(232, 103);
				output = sb.toString();

				const char* retval = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";
				Assert::AreEqual(retval, output.getValue());
			}

			TEST_METHOD(INSERT_SIMPLE)
			{
				sb.addStr("Hello");
				sb.insert('p', 3);
				output = sb.toString();
				Assert::AreEqual("Helplo", output.getValue());
			}

			TEST_METHOD(INSERT_MEDIUM)
			{
				sb.addStr("The bees sit in the trees, at night, they sit in the leaves");
				sb.insert('p', 35);
				output = sb.toString();
				Assert::AreEqual("The bees sit in the trees, at nightp, they sit in the leaves", output.getValue());
			}
	};
}
