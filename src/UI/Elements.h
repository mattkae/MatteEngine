#pragma once
#include "../MyString.h"

struct ElementAttribute;

struct Label {
	ElementAttribute* attr;
	String mText;
};

struct TextInput {
	ElementAttribute* attr;
	String mText;
};

struct Button {
	ElementAttribute* attr;
    String mText;
};
