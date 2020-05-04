#include "TextureInfo.h"
#include "BinarySerializer.h"

void TextureInfo::write(BinarySerializer& serializer) {
	serializer.writeString(fullpath);
	serializer.writeInt32(uniqueId);
}

void TextureInfo::read(BinarySerializer& serializer) {
	fullpath = serializer.readString();
	uniqueId = serializer.readInt32();
}