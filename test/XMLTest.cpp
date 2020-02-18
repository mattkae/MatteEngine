#include "CppUnitTest.h"
#include "../src/FileHelper.h"
#include "../src/FileHelper.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace File {
	TEST_CLASS(XMLTest)
	{
	private:
		const char* XML_FILE_PATH = "..\\..\\test\\assets\\xmlTestFile.xml";

	public:
		
		TEST_METHOD(OpenFileTest)
		{
			FILE* xmlFile = FileHelper::openFile(XML_FILE_PATH);
			Assert::IsNotNull(xmlFile);
			fclose(xmlFile);
		}

		TEST_METHOD(ReadTagTest)
		{
			FILE* xmlFile = FileHelper::openFile(XML_FILE_PATH);

			ReadonlyString lineBuffer;
			lineBuffer.allocate(1024);
			ReadonlyXMLToken xmlToken;
			FileHelper::XML::readTokenTag(xmlFile, lineBuffer, xmlToken);

			Assert::AreEqual((size_t)32, lineBuffer.length);

			fclose(xmlFile);
		}
	};
}