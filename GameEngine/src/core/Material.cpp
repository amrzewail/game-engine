#include "Material.hpp"

Material::~Material()
{

}

Material::Material(const Shader& shader) : shader(shader)
{
	_textures = new std::vector<Texture*>();
}

Material::Material(const Shader& shader, Texture* tex0) : Material(shader)
{
	_textures->push_back(tex0);

	LoadTextures();
}

Material::Material(const Shader& shader, Texture* tex0, Texture* tex1) : Material(shader)
{
	_textures->push_back(tex0);
	_textures->push_back(tex1);

	LoadTextures();
}

Material::Material(const Shader& shader, Texture* tex0, Texture* tex1, Texture* tex2) : Material(shader)
{
	_textures->push_back(tex0);
	_textures->push_back(tex1);
	_textures->push_back(tex2);

	LoadTextures();
}


void Material::Use() const
{
	shader.Use();
}

void Material::LoadTextures()
{
	for (auto* texture : *_textures)
	{
		if (texture->IsReadyToUse() == false)
		{
			texture->Bind();
		}
		glBindTexture(GL_TEXTURE_2D, 0); //reset the bind target
	}

}

void Material::Activate() const
{
	int activeTexture = 0;
	for (auto* tex : *_textures)
	{
		if (!tex) continue;
		glActiveTexture(GL_TEXTURE0 + activeTexture);
		if (tex->IsReadyToUse())
		{
			glBindTexture(GL_TEXTURE_2D, *tex->bindId);
		}
		shader.SetInt("tex" + std::to_string(activeTexture), activeTexture);
		activeTexture++;
	}
}
