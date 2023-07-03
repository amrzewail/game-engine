#pragma once

#include <string>

#include "Texture.hpp"
#include "shaders/Shader.h"
#include "../libs/image.h"

struct Material
{
private:
	std::vector<Texture*>* _textures;

public:
	const std::string name;

	bool enableTransparency = false;
	bool isTwoSided = false;

	const Shader& shader;

	~Material();

	Material(const std::string name, const Shader& shader);

	Material(const std::string name, const Shader& shader, Texture* tex0);

	Material(const std::string name, const Shader& shader, Texture* tex0, Texture* tex1);

	Material(const std::string name, const Shader& shader, Texture* tex0, Texture* tex1, Texture* tex2);


	void LoadTextures();
	void Activate() const;

	void Use() const;


};