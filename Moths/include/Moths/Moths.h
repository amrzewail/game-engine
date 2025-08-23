#pragma once

#pragma once

#ifdef MOTHS_EXPORTS
#define MOTHS_API __declspec(dllexport)
#else
#define MOTHS_API __declspec(dllimport)
#endif

namespace MothsEngine
{
	class MOTHS_API Moths
	{
	public:
		static void Initialize();
		static void Shutdown();
	};
}