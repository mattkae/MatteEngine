#include "FileHelper.h"

namespace FileHelper {
	namespace XML {
		const int DEFAULT_NUM_VARIABLES = 8;

		bool readTokenTag(FILE* file, ReadonlyString& buffer, ReadonlyXMLToken& outToken) {
			if (file == nullptr) {
				return false;
			}

			if (buffer.data == nullptr) {
				return false;
			}

			readNext(file, buffer);

			// Find the opening tag character
			int startIndex = 0;
			while (buffer.data[startIndex] != '<') {
				if (buffer.data[startIndex] != ' ' && buffer.data[startIndex] != '\n') {
					return false;
				}
				startIndex++;
			}

			buffer.advancePtr(startIndex);

			// Find the closing tag character
			int endIndex = startIndex + 1;
			while (buffer.data[endIndex] != '>') {
				if (endIndex == buffer.bufferSize) {
					buffer.grow(2 * buffer.bufferSize);
					fseek(file, buffer.bufferSize, SEEK_SET);
					readNext(file, buffer);
				}

				endIndex++;
			}

			// Set the buffer parameters, and point it at the beginning of the tag
			buffer.length = endIndex - startIndex;
			buffer.data[endIndex + 1] = '\0';
			buffer.advancePtr(startIndex + 1); // Pointed to the start of the name

			// Set up the token
			outToken.str = &buffer;
			outToken.name.startIndex = buffer.ptr[0];
			outToken.name.length = 0;
			buffer.movePastWhiteSpace();
			outToken.name = buffer.createPointerToChar(' ');

			// Allocate the variables array, and find them
			outToken.variables.allocate(DEFAULT_NUM_VARIABLES, sizeof(XMLValue));

			buffer.movePastWhiteSpace();

			// Get the value array
			while (!buffer.isAtEnd()) {
				XMLValue xmlValue;
				xmlValue.name = buffer.createPointerToChar('=');
				if (buffer.ptr[0] == '"') {
					buffer.advancePtr();
				}

				const char delimArray[4] = {' ', '=', '>', '"'};
				xmlValue.value = buffer.createPointerToCharArr(delimArray);
				outToken.variables.add(&xmlValue);
			}

			return true;
		}
	}
}