#include "CppUnitTest.h"
#include "../src/List.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Containers
{
	struct TestStruct {
		int foo = 12;
		int bar = 156;
	};

	TEST_CLASS(ListTest)
	{
	public:
		
		TEST_METHOD(EmptyListTest)
		{
			List<int> list;
			list.allocate(1024);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual((size_t)0, list.numElements);
			list.deallocate();
		}

		TEST_METHOD(AddToListTest)
		{
			List<int> list;
			list.allocate(1024);
			int five = 5;
			list.add(&five);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual((size_t)1, list.numElements);

			Assert::AreEqual(five, *list.getValue(0));

			int six = 6;
			list.add(&six);
			Assert::AreEqual(six, *list.getValue(1));

			list.deallocate();
		}

		TEST_METHOD(AddToListTestStruct)
		{
			List<TestStruct> list;
			list.allocate(1024);
			TestStruct inValue;
			list.add(&inValue);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual((size_t)1, list.numElements);

			TestStruct outValue = *list.getValue(0);
			Assert::AreEqual(inValue.foo, outValue.foo);
			list.deallocate();
		}

		TEST_METHOD(GrowWorks)
		{
			List<double> list;
			list.allocate(1024);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual((size_t)0, list.numElements);

			list.grow(2048);
			Assert::AreEqual((size_t)2048, list.capacity);
			Assert::AreEqual((size_t)0, list.numElements);

			list.deallocate();
		}

		TEST_METHOD(DeallocateWorks)
		{
			List<double> list;
			list.allocate(1024);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual((size_t)0, list.numElements);

			list.deallocate();

			Assert::AreEqual((size_t)0, list.capacity);
			Assert::AreEqual((size_t)0, list.numElements);
		}
	};
}