#include "Math.h"

const float Math::PI = 3.14159265358979;
const float Math::Deg2Rad = PI / 180.0f;
const float Math::Rad2Deg = 180.0f / PI;
const float Math::Epsilon = FLT_EPSILON;
const float Math::Infinity = FLT_MAX;
const float Math::NegativeInfinity = -FLT_MAX;

float Math::Abs(float f)
{
	return abs(f);
}

bool Math::Approximately(float a, float b)
{
    if (a >= b - Math::Epsilon && a <= b + Math::Epsilon)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Math::Ceil(float f)
{
    return (int)ceilf(f);
}

float Math::Clamp(float value, float min, float max)
{
    if (value > max) value = max;
    if (value < min) value = min;
    return value;
}


float Math::Clamp01(float value)
{
    return Math::Clamp(value, 0, 1);
}

float Math::Exp(float f)
{
    return expf(f);
}
int Math::Floor(float f)
{
    return (int)floorf(f);
}

float Math::InverseLerp(float a, float b, float value)
{
    return (value - a) / (b - a);
}

float Math::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float Math::Log(float f, int p)
{
    return _fdlog(f, p);
}

float Math::Log10(float f)
{
    return log10f(f);
}

float Math::Max(float a, float b)
{
    if (a > b) return a;
    else return b;
}

float Math::Min(float a, float b)
{
    if (a < b) return a;
    else return b;
}
float Math::MoveTowards(float current, float target, float maxDelta)
{
    int sign = Math::Sign(target - current);
    current += sign * maxDelta;
    if (sign > 0 && current > target) current = target;
    else if (sign < 0 && current < target) current = target;
    return current;
}

float Math::Pow(float f, float p)
{
    return powf(f, p);
}

int Math::Round(float f)
{
    return (int)roundf(f);
}

int Math::Sign(float f)
{
    if (f > 0) return 1;
    else if (f < 0) return -1;
    else return 0;
}

float Math::CopySign(float a, float b)
{
    return (b >= 0.0f) ? Math::Abs(a) : -Math::Abs(a);
}

float Math::Mod(float f, float a)
{
    return fmod(f, a);
}

float Math::Sin(float radians)
{
    return sinf(radians);
}


float Math::Cos(float radians)
{
    return cosf(radians);
}

float Math::Sqrt(float radians)
{
    return sqrtf(radians);
}

float Math::Tan(float radians)
{
    return tanf(radians);
}
float Math::Asin(float radians)
{
    return asinf(radians);
}

float Math::Atan(float radians)
{
    return atanf(radians);
}

float Math::Atan2(float y, float x)
{
    return atan2f(y, x);
}

float Math::Acos(float radians)
{
    return acosf(radians);
}