#pragma once


#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#include "Asset.h"
#include "Assets.hpp";
#include "../shaders/ShaderCompiler.hpp"

class ShaderAsset : public Asset
{
private:
	Shader* _shader;

public:

	ShaderAsset(const char* path) : Asset(path)
	{

	}

	~ShaderAsset()
	{
		delete _shader;
	}

	void Deserialize(char* bytes, size_t length)
	{
		char* _chars = new char[length + 1];
		_chars[length] = '\0';

		memcpy(_chars, bytes, length);

		std::string* _text = new std::string(_chars);

		_shader = ShaderCompiler::Compile(*_text);

		delete _text;
		delete _chars;
	}




	Shader* GetShader()
	{
		return _shader;
	}
};