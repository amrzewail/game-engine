#pragma once

#include <float.h>
#include <cstdlib>
#include <algorithm>

#include <Moths/API.h>

namespace MothsEngine
{
	class MOTHS_API Math
	{

	public:

		static const float Deg2Rad;
		static const float Rad2Deg;
		static const float PI;
		static const float Epsilon;
		static const float Infinity;
		static const float NegativeInfinity;

		static bool Approximately(float a, float b);

		static int Ceil(float f);
		static int Floor(float f);
		static int Round(float f);
		static int Sign(float f);
		static float CopySign(float a, float b);

		static float Abs(float f);
		static float Clamp(float value, float min, float max);
		static float Clamp01(float value);
		static float Exp(float f);

		static float InverseLerp(float a, float b, float value);
		static float Lerp(float a, float b, float t);
		static float Log(float f, int p);
		static float Log10(float f);
		static float Max(float a, float b);
		static float Min(float a, float b);
		static float MoveTowards(float current, float target, float maxDelta);
		static float Pow(float f, float p);
		static float Sqrt(float f);
		static float Mod(float f, float a);

		static float Sin(float radians);
		static float Cos(float radians);
		static float Tan(float radians);
		static float Asin(float radians);
		static float Atan(float radians);
		static float Atan2(float y, float x);
		static float Acos(float radians);
	};
}