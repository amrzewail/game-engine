#pragma once
#include <glad/glad.h>

struct Texture
{
private:
	int _width;
	int _height;
	int _channels;
	unsigned char* _data;
	int _textureUnit = 0;

public:

	int locationId = 0;

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
	}

	Texture(unsigned char* data, int width, int height, int channels)
	{
		_data = data;
		_width = width;
		_height = height;
		_channels = channels;
	}

	~Texture()
	{
		//delete[] _data;
	}
};