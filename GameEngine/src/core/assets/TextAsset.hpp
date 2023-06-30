#pragma once

#include "Asset.h"

using std::allocator;

class TextAsset : public Asset
{
private:
	char* _chars;
	std::string* _text;

public:

	~TextAsset()
	{
		delete[] _chars;
	}

	void Deserialize(char* bytes, size_t length)
	{
		_chars = new char[length + 1];
		_chars[length] = '\0';

		memcpy(_chars, bytes, length);

		_text = new std::string(_chars);
	}


	std::string GetText()
	{
		return *_text;
	}

	char* GetChars()
	{
		return _chars;
	}
};