#pragma once

#include <vector>
#include "Material.hpp"
#include "Mesh.hpp"
#include "Component.hpp"
#include "GameObject.hpp"

class MeshRenderer : public Component
{
public:

	Mesh* mesh;
	std::vector<Material*>* materials;

	MeshRenderer()
	{
		materials = new std::vector<Material*>();
	}

	~MeshRenderer()
	{
		delete materials;
	}

	void Render(Camera& camera) const
	{
		const Matrix4x4* projection = camera.ProjectionMatrix();
		const Matrix4x4* worldMatrix = gameObject->transform->LocalToWorldMatrix();
		
		for (auto* material : *materials)
		{
			if (material->enableTransparency)
			{
				glEnable(GL_BLEND);
			}

			material->Use();

			material->shader.SetMatrix4x4("model", *worldMatrix);
			material->shader.SetMatrix4x4("PV", *projection);

			material->Activate();
		}
		mesh->Draw();

		glDisable(GL_BLEND);
	}
};