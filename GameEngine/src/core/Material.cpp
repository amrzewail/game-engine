#include "Material.hpp"

Material::~Material()
{
	delete _textures;
	delete _assignedColors;
}

Material::Material(const Material& source) : Material(source.name, source.shader)
{
	for (auto* texture : *source._textures) _textures->push_back(texture);
	for (auto const& [key, value] : *source._assignedColors) (*_assignedColors)[key] = Color(value);
}

Material& Material::operator =(const Material& rhs)
{
	if (this != &rhs)
	{
		_textures->clear();
		_assignedColors->clear();

		for (auto* texture : *rhs._textures) _textures->push_back(texture);
		for (auto const& [key, value] : *rhs._assignedColors) (*_assignedColors)[key] = Color(value);
	}
	return *this;
}

Material::Material(const std::string name, const Shader& shader) : name(name), shader(shader)
{
	_textures = new std::vector<Texture*>();
	_assignedColors = new std::unordered_map<string, Color>();
}

Material::Material(const std::string name, const Shader& shader, Texture* tex0) : Material(name, shader)
{
	_textures->push_back(tex0);

	LoadTextures();
}

Material::Material(const std::string name, const Shader& shader, Texture* tex0, Texture* tex1) : Material(name, shader)
{
	_textures->push_back(tex0);
	_textures->push_back(tex1);

	LoadTextures();
}

Material::Material(const std::string name, const Shader& shader, Texture* tex0, Texture* tex1, Texture* tex2) : Material(name, shader)
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
	for (auto const& [key, value] : *_assignedColors) shader.SetColor(key, value);

	int activeTexture = 0;
	for (auto* tex : *_textures)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
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


void Material::SetColor(const std::string& name, const Color& c)
{
	(* _assignedColors)[name] = c;
}
