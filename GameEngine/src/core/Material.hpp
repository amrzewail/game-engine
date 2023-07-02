#pragma once
#include "Texture.hpp"
#include "shaders/Shader.h"
#include "../libs/image.h"

struct Material
{
private:
	std::vector<Texture*>* _textures;

public:

	bool enableTransparency = false;

	const Shader& shader;

	~Material();

	Material(const Shader& shader);

	Material(const Shader& shader, Texture* tex0);

	Material(const Shader& shader, Texture* tex0, Texture* tex1);

	Material(const Shader& shader, Texture* tex0, Texture* tex1, Texture* tex2);


	void LoadTextures();
	void Activate() const;

	void Use() const;


};