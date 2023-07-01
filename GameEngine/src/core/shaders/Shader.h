#pragma once

#include <glad/glad.h>

#include <string>

#include "../Vector.h"
#include "../assets/TextAsset.hpp"
#include "../assets/Assets.hpp"
#include "../Color.h"
#include "../Matrix4x4.hpp"


class Shader
{
public:

	// the program ID
	const unsigned int id;

	static Shader Create(const char* vertPath, const char* fragPath);

	//// constructor reads and builds the shader
	explicit Shader(const unsigned int _id) : id(_id) {}

	//// use/activate the shader
	void Use() const;

	//// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVector(const std::string& name, float x, float y, float z) const;
	void SetVector(const std::string& name, const Vector& v) const;
	void SetColor(const std::string& name, const Color& c) const;
	void SetMatrix4x4(const std::string& name, const Matrix4x4& m) const;
};