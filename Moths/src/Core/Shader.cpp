//#define ENABLE_LOGGING

#include <iostream>
#include <Moths/Core/Shader.h>

using namespace MothsEngine;

namespace
{

	// Returns true iff successful
	bool CheckShaderCompilation(const unsigned int shaderId)
	{
		int success;
		// TODO(nick.c): This doesn't need to get allocated repeatedly... not a biggie
		// though.
		char infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return success == 1;
	}

	bool CheckProgramLinking(const unsigned int programId)
	{
		int success;
		char infoLog[512];
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			std::cout << "Error linking program\n" << infoLog << std::endl;
		}
		return success == 1;
	}

} // namespace


const string Shader::FLAG_TRANSPARENT = "TRANSPARENT";
const string Shader::FLAG_Blend_SrcAlpha_OneMinusSrcAlpha = "Blend_SrcAlpha_OneMinusSrcAlpha";
const string Shader::FLAG_DEPTH_TEST_OFF = "DEPTH_TEST_OFF";
const string Shader::FLAG_CULL_FACE_OFF = "CULL_FACE_OFF";


Shader* Shader::Create(const char* vertShader, const char* fragShader)
{
	const char* vShaderCode = vertShader;
	const char* fShaderCode = fragShader;

	const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	if (!CheckShaderCompilation(vertexShader) ||
		!CheckShaderCompilation(fragmentShader))
	{
		exit(1);
	}

	const unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	if (!CheckProgramLinking(shaderProgram))
	{
		exit(1);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return new Shader(shaderProgram);
}

Shader::~Shader()
{
	delete _defines;
}

void Shader::Use() const { glUseProgram(id); }

void Shader::SetBool(const std::string& name, bool value) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	glUniform1i(loc, (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	glUniform1i(loc, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	glUniform1f(loc, value);
}

void Shader::SetVector(const std::string& name, const Vector& v) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::SetVector2(const std::string& name, const Vector2& v) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	glUniform2f(loc, v.x, v.y);
}

void Shader::SetColor(const std::string& name, const Color& c) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	glUniform4f(loc, c.r, c.g, c.b, c.a);
}

void Shader::SetMatrix4x4(const std::string& name, const Matrix4x4& m) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data());
}


void Shader::SetMatrix4x4Array(const std::string& name, const Matrix4x4 array[], int length) const
{
	if (length <= 0) return;
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
#ifdef ENABLE_LOGGING
		std::cerr << "Couldn't find " << name << std::endl;
#endif
		return;
	}
	std::vector<float*> mats;
	for (int i = 0; i < length; i++)
	{
		mats.push_back(array[i].Data());
	}
	glUniformMatrix4fv(loc, length, GL_FALSE, *mats.data());
}