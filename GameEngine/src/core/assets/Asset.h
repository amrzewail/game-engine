#pragma once

#include <string>
#include "../utils/Guid.hpp"

class Asset
{

public:

	const std::string* guid;
	const char* path;

	Asset(const char* path) : path(path), guid(Guid::NewGuid()->ToStringPtr())
	{

	}

	virtual ~Asset()
	{
		delete guid;
	}

	virtual void Deserialize(char* bytes, size_t length) = 0;

};