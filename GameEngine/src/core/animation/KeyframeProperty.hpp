#pragma once
#include "../Vector.h"
#include "../Quaternion.hpp"


struct KeyframeProperty
{

public:
	Matrix4x4 TRS;

	KeyframeProperty()
	{
		TRS = Matrix4x4::Identity();
	}

	KeyframeProperty(KeyframeProperty& source)
	{
		TRS = source.TRS;
	}

	KeyframeProperty& operator=(const KeyframeProperty& rhs)
	{
		if (this == &rhs) return *this;
		TRS = rhs.TRS;
		return *this;
	}
};