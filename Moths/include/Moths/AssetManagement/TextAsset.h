#pragma once

#include <Moths/API.h>
#include <Moths/AssetManagement/Asset.h>

using std::allocator;

namespace MothsEngine
{

	class MOTHS_API TextAsset : public Asset
	{
	private:
		char* _chars;
		std::string* _text;

	public:

		TextAsset(const char* path) : Asset(path)
		{

		}

		~TextAsset()
		{
			delete[] _chars;
		}

		inline void Deserialize(char* bytes, size_t length)
		{
			_chars = new char[length + 1];
			_chars[length] = '\0';

			memcpy(_chars, bytes, length);

			_text = new std::string(_chars);
		}


		inline std::string GetText()
		{
			return *_text;
		}

		inline char* GetChars()
		{
			return _chars;
		}
	};
}