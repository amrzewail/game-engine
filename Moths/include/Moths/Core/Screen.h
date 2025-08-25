#pragma once

#include <Moths/API.h>

namespace MothsEngine
{
	class MOTHS_API Screen
	{
	private:
		static int _width;
		static int _height;

	public:
		static void SetResolution(const int width, const int height);
		static int Width();
		static int Height();
	};
}