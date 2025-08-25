#pragma once

#include <string>
#include <unordered_map>
#include "../../libs/image.h"

#include <Moths/API.h>
#include <Moths/Core/Texture.h>
#include <Moths/Core/Color.h>
#include <Moths/Core/Shader.h>
#include <Moths/AssetManagement/Assets.h>
#include <Moths/AssetManagement/TextureAsset.h>

namespace MothsEngine
{
	struct MOTHS_API Material
	{
	private:
		std::vector<Texture*> _textures;
		std::unordered_map<std::string, Color> _assignedColors;

	public:
		const std::string name;

		bool enableTransparency = false;
		bool isTwoSided = false;

		const Shader& shader;

		~Material();

		Material(const Material& source);

		Material& operator =(const Material& rhs);

		Material(const std::string name, const Shader& shader);

		Material(const std::string name, const Shader& shader, Texture* tex0);

		Material(const std::string name, const Shader& shader, Texture* tex0, Texture* tex1);

		Material(const std::string name, const Shader& shader, Texture* tex0, Texture* tex1, Texture* tex2);


		void LoadTextures();
		void Activate() const;

		void Use() const;

		//// utility uniform functions
		void SetBool(const std::string& name, const bool value);
		void SetInt(const std::string& name, const int value);
		void SetFloat(const std::string& name, const float value);
		void SetVector(const std::string& name, const Vector& v);
		void SetVector2(const std::string& name, const Vector2& v);
		void SetColor(const std::string& name, const Color& c);
		void SetMatrix4x4(const std::string& name, const Matrix4x4& m);
	};
}