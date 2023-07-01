#pragma once

#include "Asset.h"

#include "../../libs/image.h"
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
		stbi_image_free(_texture->Data());
		delete _texture;
	}

	void Deserialize(char* bytes, size_t length)
	{
		int width, height, nrComponents;

		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load_from_memory((unsigned char*)bytes, length, &width, &height, &nrComponents, 0);

		_texture = new Texture(data, width, height, nrComponents);
	}

	Texture& Tex()
	{
		return *_texture;
	}

};