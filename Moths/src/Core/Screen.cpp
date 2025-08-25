#include <Moths/Core/Screen.h>

using namespace MothsEngine;

int Screen::_width = 1280;
int Screen::_height = 720;

void Screen::SetResolution(const int width, const int height)
{
	_width = width;
	_height = height;
}

int Screen::Width()
{
	return _width;
}

int Screen::Height()
{
	return _height;
}