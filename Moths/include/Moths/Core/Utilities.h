#pragma once
#include <string>

namespace MothsEngine::Internal
{

	static const char* ExtractVersion(const char* full)
	{
		std::string p1 = full;
		static std::string p2;
		p2.clear();
		p2 = p1.substr(p1.find_last_of("/\\") + 1, (p1.length() - p1.find_last_of("/\\")) - (p1.length() - p1.find_last_of(".")) + 1);
		return p2.c_str();
	}


}