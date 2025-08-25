#pragma once

#include <string>

#include <Moths/API.h>
#include <Moths/Core/Guid.h>

namespace MothsEngine
{
	class MOTHS_API Asset
	{
	public:

		const Guid* guid;
		const char* path;

		Asset(const char* path) : path(path), guid(Guid::NewGuid())
		{

		}

		virtual ~Asset()
		{
			delete guid;
		}

		virtual void Deserialize(char* bytes, size_t length) = 0;
	};
}