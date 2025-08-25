#pragma once

#include <fstream>
#include <string>

#include <Moths/API.h>

namespace MothsEngine
{
	class MOTHS_API Debug
	{
	public:

		static inline void Log(const std::string msg)
		{
			Log(msg.c_str());
		}

#ifdef _DEBUG
		static inline void Log(const char* msg)
		{
			std::ofstream logStream;
			logStream.open("log.txt", std::ofstream::app | std::ofstream::out);
			logStream << msg;
			logStream.close();

			std::cout << msg << std::endl;
		}
#else
		static inline void Log(const char* msg)
		{
			std::cout << msg << std::endl;
		}
#endif
	};
}