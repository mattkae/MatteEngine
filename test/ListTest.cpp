#include "CppUnitTest.h"
#include "../src/List.h"
#include "../src/List.cpp"

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
			List list;
			list.allocate(1024, sizeof(int));
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual(sizeof(int), list.elementSize);
			Assert::AreEqual((size_t)(1024 * sizeof(int)), list.sizeBytes);
			Assert::AreEqual((size_t)0, list.numElements);
			list.deallocate();
		}

		TEST_METHOD(AddToListTest)
		{
			List list;
			list.allocate(1024, sizeof(int));
			int five = 5;
			list.add(&five);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual(sizeof(int), list.elementSize);
			Assert::AreEqual((size_t)(1024 * sizeof(int)), list.sizeBytes);
			Assert::AreEqual((size_t)1, list.numElements);

			int value = ((int*)(list.data))[0];
			Assert::AreEqual(five, value);

			int six = 6;
			list.add(&six);
			value = ((int*)(list.data))[1];
			Assert::AreEqual(six, value);

			list.deallocate();
		}

		TEST_METHOD(AddToListTestStruct)
		{
			List list;
			list.allocate(1024, sizeof(TestStruct));
			TestStruct inValue;
			list.add(&inValue);
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual(sizeof(TestStruct), list.elementSize);
			Assert::AreEqual((size_t)(1024 * sizeof(TestStruct)), list.sizeBytes);
			Assert::AreEqual((size_t)1, list.numElements);

			TestStruct outValue = ((TestStruct*)(list.data))[0];
			Assert::AreEqual(inValue.foo, outValue.foo);
			list.deallocate();
		}

		TEST_METHOD(GrowWorks)
		{
			List list;
			list.allocate(1024, sizeof(double));
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual(sizeof(double), list.elementSize);
			Assert::AreEqual((size_t)(1024 * sizeof(double)), list.sizeBytes);
			Assert::AreEqual((size_t)0, list.numElements);

			list.grow(2048);
			Assert::AreEqual((size_t)2048, list.capacity);
			Assert::AreEqual(sizeof(double), list.elementSize);
			Assert::AreEqual((size_t)(2048 * sizeof(double)), list.sizeBytes);
			Assert::AreEqual((size_t)0, list.numElements);

			list.deallocate();
		}

		TEST_METHOD(DeallocateWorks)
		{
			List list;
			list.allocate(1024, sizeof(double));
			Assert::AreEqual((size_t)1024, list.capacity);
			Assert::AreEqual(sizeof(double), list.elementSize);
			Assert::AreEqual((size_t)(1024 * sizeof(double)), list.sizeBytes);
			Assert::AreEqual((size_t)0, list.numElements);

			list.deallocate();

			Assert::AreEqual((size_t)0, list.capacity);
			Assert::AreEqual((size_t)0, list.elementSize);
			Assert::AreEqual((size_t)0, list.sizeBytes);
			Assert::AreEqual((size_t)0, list.numElements);
		}
	};
}