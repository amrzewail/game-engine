#pragma once
#include <glad/glad.h>
#include <iostream>
#include "../../libs/image.h"

#include <Moths/API.h>

namespace MothsEngine
{
	struct MOTHS_API Texture
	{
	private:
		int _width;
		int _height;
		int _channels;
		unsigned char* _data;
		int _textureUnit = 0;
		bool _isReady = false;

	public:
		GLuint* bindId;

		int Width() const
		{
			return _width;
		}

		int Height() const
		{
			return _height;
		}

		unsigned char* Data() const
		{
			return _data;
		}

		int InternalFormat()
		{
			switch (_channels)
			{
			case 1: return GL_RED;
			case 2:return GL_RG;
			case 3: return GL_RGB;
			case 4: return GL_RGBA;
			}
			return GL_RGB;
		}

		Texture()
		{
			_data = 0;
			_width = _height = _channels = 0;
			bindId = new GLuint();
		}

		Texture(char* bytes, int length) : Texture()
		{
			int width, height, channels;

			stbi_set_flip_vertically_on_load(false);
			_data = stbi_load_from_memory((unsigned char*)bytes, length, &width, &height, &channels, 0);
			_width = width;
			_height = height;
			_channels = channels;
		}


		~Texture()
		{
			if (!IsReadyToUse()) return;

			std::cout << "Unloaded texture from gpu memory: " << *bindId << std::endl;

			glDeleteTextures(1, bindId);
			delete bindId;
		}

		void Bind()
		{
			if (_isReady) return;

			glGenTextures(1, bindId);

			glBindTexture(GL_TEXTURE_2D, *bindId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat(), Width(), Height(), 0, InternalFormat(), GL_UNSIGNED_BYTE, Data());

			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(_data);

			std::cout << "Loaded texture to gpu memory: " << *bindId << std::endl;

			_isReady = true;
		}

		bool IsReadyToUse()
		{
			return _isReady;
		}
	};
}