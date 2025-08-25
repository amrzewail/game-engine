#pragma once

#include <Moths/API.h>
#include <Moths/Core/Texture.h>
#include <Moths/AssetManagement/Asset.h>

namespace MothsEngine
{
	class MOTHS_API TextureAsset : public Asset
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

		inline void Deserialize(char* bytes, size_t length)
		{
			_texture = new Texture(bytes, length);
		}

		inline Texture* GetTexture()
		{
			return _texture;
		}

	};
}