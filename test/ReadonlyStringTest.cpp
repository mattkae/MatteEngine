#include "CppUnitTest.h"
#include "../src/ReadonlyString.h"
#include "../src/ReadonlyString.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace String
{
	TEST_CLASS(ReadonlyStringTest)
	{
	public:
		
		TEST_METHOD(AllocateTest)
		{
			ReadonlyString str;
			str.allocate(1024);
			Assert::AreEqual((size_t)1024, str.bufferSize);

			str.free();
			Assert::AreEqual((size_t)0, str.bufferSize);
		}

		TEST_METHOD(GrowTest)
		{
			ReadonlyString str;
			str.allocate(1024);
			Assert::AreEqual((size_t)1024, str.bufferSize);

			str.grow(1024 * 2);
			Assert::AreEqual((size_t)1024 * 2, str.bufferSize);
			
			str.free();
			Assert::AreEqual((size_t)0, str.bufferSize);

			str.setStr("A new string");
			Assert::AreEqual((size_t)13, str.bufferSize);
			Assert::AreEqual(std::string("A new string"), std::string(str.data));
		}

		TEST_METHOD(SetTestEmpty)
		{
			ReadonlyString str;
			str.setStr("A new string");
			Assert::AreEqual((size_t)13, str.bufferSize);
			Assert::AreEqual(std::string("A new string"), std::string(str.data));
		}

		TEST_METHOD(SetTestReplace)
		{
			ReadonlyString str;

			str.allocate(1024);
			Assert::AreEqual((size_t)1024, str.bufferSize);

			str.setStr("A new string");
			Assert::AreEqual((size_t)1024, str.bufferSize);
			Assert::AreEqual(std::string("A new string"), std::string(str.data));
		}

		TEST_METHOD(AdvancePtrPastChar)
		{
			ReadonlyString str;

			str.setStr("A new string");
			str.advancePtrPastChar('e');
			Assert::AreEqual((size_t)4, str.ptrPosition);
			Assert::AreEqual('w', str.ptr[0]);
		}

		TEST_METHOD(AdvancePtr)
		{
			ReadonlyString str;

			str.setStr("A new string");
			str.advancePtr(4);
			Assert::AreEqual((size_t)4, str.ptrPosition);
			Assert::AreEqual('w', str.ptr[0]);
		}

		TEST_METHOD(PointerToCharSuccess)
		{
			ReadonlyString str;

			str.setStr("A new string");
			ReadonlyStrPtr ptr = str.createPointerToChar('e');

			Assert::AreEqual((size_t)0, ptr.startIndex);
			Assert::AreEqual((size_t)3, ptr.length);
		}

		TEST_METHOD(PointerToCharFail)
		{
			ReadonlyString str;

			str.setStr("A new string");
			ReadonlyStrPtr ptr = str.createPointerToChar('z');

			Assert::AreEqual((size_t)0, ptr.startIndex);
			Assert::AreEqual((size_t)0, ptr.length);
		}

		TEST_METHOD(PointerToCharArrSuccess)
		{
			ReadonlyString str;

			str.setStr("A new string");
			const char arr[3] = {'x', 'y', 'r'};
			ReadonlyStrPtr ptr = str.createPointerToCharArr(arr);

			Assert::AreEqual((size_t)0, ptr.startIndex);
			Assert::AreEqual((size_t)8, ptr.length);
		}

		TEST_METHOD(PointerToCharArrFail)
		{
			ReadonlyString str;

			str.setStr("A new string");
			const char arr[3] = {'x', 'y', 'z'};
			ReadonlyStrPtr ptr = str.createPointerToCharArr(arr);

			Assert::AreEqual((size_t)0, ptr.startIndex);
			Assert::AreEqual((size_t)0, ptr.length);
		}
	};
}
