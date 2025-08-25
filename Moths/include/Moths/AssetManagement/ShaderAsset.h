#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iostream>

#include <Moths/API.h>
#include <Moths/Core/Shader.h>

#include <Moths/AssetManagement/Asset.h>
#include <Moths/AssetManagement/Assets.h>
#include <Moths/AssetManagement/ShaderCompiler.h>

namespace MothsEngine
{

	class MOTHS_API ShaderAsset : public Asset
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

		inline void Deserialize(char* bytes, size_t length)
		{
			char* _chars = new char[length + 1];
			_chars[length] = '\0';

			memcpy(_chars, bytes, length);

			std::string* _text = new std::string(_chars);

			_shader = Internal::ShaderCompiler::Compile(*_text);

			delete _text;
			delete _chars;
		}

		inline Shader* GetShader()
		{
			return _shader;
		}
	};
}