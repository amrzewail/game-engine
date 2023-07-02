#include <iostream>

#include "Shader.h"

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

Shader Shader::Create(const char* vertPath, const char* fragPath)
{
	TextAsset& vertTxt = Assets::Load<TextAsset>(vertPath);
	TextAsset& fragTxt = Assets::Load<TextAsset>(fragPath);

	const char* vShaderCode = vertTxt.GetChars();
	const char* fShaderCode = fragTxt.GetChars();

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

	return Shader(shaderProgram);
}

void Shader::Use() const { glUseProgram(id); }

void Shader::SetBool(const std::string& name, bool value) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}
	glUniform1i(loc, (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}
	glUniform1i(loc, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}
	glUniform1f(loc, value);
}

void Shader::SetVector(const std::string& name, float x, float y, float z) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}
	glUniform3f(loc, x, y, z);
}

void Shader::SetVector(const std::string& name, const Vector& v) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}
	glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::SetColor(const std::string& name, const Color& c) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}
	glUniform4f(loc, c.r, c.g, c.b, c.a);
}

void Shader::SetMatrix4x4(const std::string& name, const Matrix4x4& m) const
{
	const int loc = glGetUniformLocation(id, name.c_str());
	if (loc < 0)
	{
		std::cerr << "Couldn't find " << name << std::endl;
		exit(1);
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data());
}