#pragma once

#include "Asset.h"

#include "../Texture.hpp"


class TextureAsset : public Asset
{
private:
	Texture* _texture;

public:

	TextureAsset(const char* path) : Asset(path)
	{

	}

	~TextureAsset()
	{
		delete _texture;
	}

	void Deserialize(char* bytes, size_t length)
	{
		_texture = new Texture(bytes, length);
	}

	Texture* GetTexture()
	{
		return _texture;
	}

};