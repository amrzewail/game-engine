#pragma once

#ifdef MOTHS_EXPORTS
#define MOTHS_API __declspec(dllexport)
#else
#define MOTHS_API __declspec(dllimport)
#endif