#pragma once


#include <bitset>
#include <functional>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include "Asset.h";

using std::allocator;

using namespace std;

class Assets
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

	static bool ExistsByGuid(string guid, int* index)
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
		const string* guid = asset.guid;
		int assetIndex;
		if (!ExistsByGuid(*guid, &assetIndex)) return;
		delete _loadedAssets[assetIndex];
		_loadedAssets.erase(_loadedAssets.begin() + assetIndex);
		_loadedHashPaths.erase(_loadedHashPaths.begin() + assetIndex);
		std::cout << "Unloaded Asset: " << guid << std::endl;
	}

	template<typename T>
	static T& Load(const char* path)
	{
		string pathStr = string(path);


		std::hash<string> myhash;
		size_t pathHash = myhash(pathStr);

		int assetIndex;

		if (ExistsByHash(pathHash, &assetIndex))
		{
			T* asset = static_cast<T*>(_loadedAssets[assetIndex]);
			return *asset;
		}

		std::ifstream infile("assets/" + pathStr, std::ios_base::binary);


		infile.seekg(0, std::ios::end);
		size_t length = infile.tellg();
		infile.seekg(0, std::ios::beg);

		allocator<char> alloc;
		char* bytes = alloc.allocate(length);

		infile.read(bytes, length);

		infile.close();


		T* asset = new T(path);
		asset->Deserialize(bytes, length);

		std::string guid = *asset->guid;

		std::cout << "Loaded Asset: " << path << " guid: " << guid << std::endl;

		_loadedAssets.push_back(asset);
		_loadedHashPaths.push_back(pathHash);

		alloc.deallocate(bytes, length);
		
		return *asset;
	}
};