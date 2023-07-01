#pragma once

#include <memory>
#include <unordered_map>
#include <fstream>

#include "Asset.h";

using std::allocator;

class Assets
{
private:
	static std::unordered_map<const char*, Asset*> _loadedAssets;

	static bool Exists(const char* path)
	{
		return _loadedAssets.find(path) != _loadedAssets.end();
	}

public:

	static void Unload(const char* path)
	{
		if (!Exists(path)) return;
		Asset* asset = _loadedAssets[path];
		delete asset;
	}

	template<typename T>
	static T& Load(const char* path)
	{
		if (Exists(path))
		{
			T* asset = static_cast<T*>(_loadedAssets[path]);
			return *asset;
		}

		std::ifstream infile("assets/" + std::string(path), std::ios_base::binary);

		infile.seekg(0, std::ios::end);
		size_t length = infile.tellg();
		infile.seekg(0, std::ios::beg);

		allocator<char> alloc;
		char* bytes = alloc.allocate(length);

		infile.read(bytes, length);

		infile.close();

		T* asset = new T(path);
		asset->Deserialize(bytes, length);

		_loadedAssets[path] = asset;

		alloc.deallocate(bytes, length);

		return *asset;
	};
};