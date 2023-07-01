#pragma once
#include "Texture.hpp"
#include "shaders/Shader.h"
#include "../libs/image.h"

struct Material
{

public:
	const Shader& shader;
	std::vector<Texture> textures;


	Material(const Shader& shader) : shader(shader)
	{
		textures = std::vector<Texture>();
	}

	Material(const Shader& shader, Texture tex0) : Material(shader)
	{
		textures.push_back(tex0);

		LoadTextures();
	}

	Material(const Shader& shader, Texture tex0, Texture tex1) : Material(shader)
	{
		textures.push_back(tex0);
		textures.push_back(tex1);

		LoadTextures();
	}

	Material(const Shader& shader, Texture tex0, Texture tex1, Texture tex2) : Material(shader)
	{
		textures.push_back(tex0);
		textures.push_back(tex1);
		textures.push_back(tex2);

		LoadTextures();
	}

	~Material()
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			glDeleteTextures(1, (GLuint*)(&textures[i].locationId));
		}
	}

	void LoadTextures()
	{
		//GLuint* textureIds = (GLuint*)malloc(sizeof(GLuint) * textures.size());
		//glGenTextures(textures.size(), textureIds);

		for (int i = 0; i < textures.size(); i++)
		{
			Texture* texture = &textures[i];
			GLuint id;
			glGenTextures(1, &id);
			texture->locationId = id;
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, texture->InternalFormat(), texture->Width(), texture->Height(), 0, texture->InternalFormat(), GL_UNSIGNED_BYTE, texture->Data());
			//glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Use() const
	{
		shader.Use();
	}


	void Activate() const
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, textures[i].locationId);
			glActiveTexture(GL_TEXTURE0 + i);
			shader.SetInt("tex" + std::to_string(i), i);
		}
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
};