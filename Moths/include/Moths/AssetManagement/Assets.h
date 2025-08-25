#pragma once


#include <bitset>
#include <functional>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include <Moths/API.h>
#include <Moths/Core/Guid.h>
#include <Moths/AssetManagement/Asset.h>;

namespace MothsEngine
{
	class MOTHS_API Assets
	{
	private:
		static std::vector<size_t> _loadedHashPaths;
		static std::vector<Asset*> _loadedAssets;

		static bool ExistsByHash(size_t hash, int* index)
		{
			*index = 0;
			for (auto s : _loadedHashPaths)
			{
				if (s == hash) return true;
				*index = *index + 1;
			}
			return false;
		}

		static bool ExistsByGuid(Guid guid, int* index)
		{
			*index = 0;
			for (auto a : _loadedAssets)
			{
				if (*a->guid == guid) return true;
				*index = *index + 1;
			}
			return false;
		}

	public:

		static void UnloadAll()
		{
			while (_loadedAssets.size() > 0)
			{
				Unload(*_loadedAssets[0]);
			}
		}

		static void Unload(Asset& asset)
		{
			const Guid guid = *asset.guid;
			int assetIndex;
			if (!ExistsByGuid(guid, &assetIndex)) return;
			delete _loadedAssets[assetIndex];
			_loadedAssets.erase(_loadedAssets.begin() + assetIndex);
			_loadedHashPaths.erase(_loadedHashPaths.begin() + assetIndex);
			std::cout << "Unloaded Asset: " << guid.ToString() << std::endl;
		}

		template<typename T>
		static T& LoadInternal(const char* path)
		{
			std::string pathStr = std::string(path);


			std::hash<std::string> myhash;
			size_t pathHash = myhash(pathStr);

			int assetIndex;

			if (ExistsByHash(pathHash, &assetIndex))
			{
				T* asset = static_cast<T*>(_loadedAssets[assetIndex]);
				return *asset;
			}

			std::ifstream infile(pathStr, std::ios_base::binary);

			infile.seekg(0, std::ios::end);
			size_t length = infile.tellg();
			infile.seekg(0, std::ios::beg);

			std::allocator<char> alloc;
			char* bytes = alloc.allocate(length);

			infile.read(bytes, length);

			infile.close();

			T* asset = new T(path);
			asset->Deserialize(bytes, length);

			Guid guid = *asset->guid;

			std::cout << "Loaded Asset: " << path << " guid: " << guid.ToString() << std::endl;

			_loadedAssets.push_back(asset);
			_loadedHashPaths.push_back(pathHash);

			alloc.deallocate(bytes, length);

			return *asset;
		}

		template<typename T>
		static T& Load(const char* path)
		{
			return LoadInternal<T>(("assets/" + std::string(path)).c_str());
		}
	};
}