#pragma once

#ifndef NOMINMAX
#  define NOMINMAX
#endif

#define _HAS_STD_BYTE 0

#include <string>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include <objbase.h>
#include <memory>
#include <unordered_map>
#include <chrono>

#include <Moths/API.h>

#include <Moths/Core/Quad.h>
#include <Moths/Core/Debug.h>
#include <Moths/Core/Math.h>
#include <Moths/Core/Vector.h>
#include <Moths/Core/Vector2.h>
#include <Moths/Core/Vector4.h>
#include <Moths/Core/Matrix4x4.h>
#include <Moths/Core/Color.h>
#include <Moths/Core/Transform.h>
#include <Moths/Core/Screen.h>
#include <Moths/Core/Material.h>
#include <Moths/Core/Guid.h>

#include <Moths/Entity/Camera.h>
#include <Moths/Entity/GameObject.h>

#include <Moths/Component/Component.h>
#include <Moths/Component/MeshRenderer.h>

#include <Moths/AssetManagement/Assets.h>
#include <Moths/AssetManagement/TextAsset.h>
#include <Moths/AssetManagement/ModelAsset.h>
#include <Moths/AssetManagement/TextureAsset.h>
#include <Moths/AssetManagement/ShaderAsset.h>
#include <Moths/AssetManagement/GameObjectAsset.h>

#include <Moths/AssetManagement/Data/Model.h>

namespace MothsEngine
{
	class MOTHS_API Moths
	{
	private:
		static GLFWwindow* Window;

	public:
		static void Initialize();

		static void Run();
	};
}