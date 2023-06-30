#pragma once

class Asset
{

public:

	virtual void Deserialize(char* bytes, size_t length) = 0;

};