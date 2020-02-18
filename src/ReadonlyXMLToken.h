#ifndef READONLY_XML_TOKEN_H
#define READONLY_XML_TOKEN_H
#include "ReadonlyString.h"
#include "List.h"

struct XMLValue {
	ReadonlyStrPtr name;
	ReadonlyStrPtr value;
};

struct ReadonlyXMLToken {
	ReadonlyString* str = nullptr;

	// Tag name
	ReadonlyStrPtr name;

	// Variables: resizable array
	List variables;
};

#endif