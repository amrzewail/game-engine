#pragma once

#include <Moths/API.h>
#include <Moths/Core/Vector.h>
#include <Moths/Core/Quaternion.h>
#include <Moths/Core/Matrix4x4.h>

namespace MothsEngine
{
	struct MOTHS_API KeyframeProperty
	{

	public:
		Matrix4x4 TRS;

		KeyframeProperty()
		{
			TRS = Matrix4x4::Identity();
		}

		KeyframeProperty(const KeyframeProperty& source) = default;

		inline KeyframeProperty& operator=(const KeyframeProperty& rhs)
		{
			if (this == &rhs) return *this;
			TRS = rhs.TRS;
			return *this;
		}
	};
}