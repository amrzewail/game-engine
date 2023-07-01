#pragma once

#include <vector>
#include "Material.hpp"
#include "Mesh.hpp"

class MeshRenderer
{
public:

	Mesh* mesh;
	std::vector<Material*> materials;

	MeshRenderer()
	{

	}

	void Render() const
	{
		for (int i = 0; i < materials.size(); i++)
		{
			materials[i]->Activate();
		}
		mesh->Draw();
	}
};