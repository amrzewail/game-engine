#pragma once

#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000
#include <Windows.h>
#include <iostream>
#include <string>

struct Guid
{
private:
	char* _c_str;

public:
	static Guid* NewGuid()
	{
		return new Guid();
	}


	Guid()
	{
		UUID uuid;
		UuidCreate(&uuid);
		UuidToStringA(&uuid, (RPC_CSTR*)&_c_str);
	}

	~Guid()
	{
		RpcStringFreeA((RPC_CSTR*)&_c_str);
	}

	std::string ToString() const
	{
		if (!_c_str || _c_str == nullptr) return "";
		return _c_str;
	}

	std::string* ToStringPtr() const
	{
		return new std::string(_c_str);
	}
};