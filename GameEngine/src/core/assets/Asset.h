#pragma once

class Asset
{

public:

	const char* path;

	Asset(const char* path) : path(path)
	{

	}

	virtual ~Asset()
	{

	}

	virtual void Deserialize(char* bytes, size_t length) = 0;

};