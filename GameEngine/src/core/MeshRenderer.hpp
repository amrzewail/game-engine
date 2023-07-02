#pragma once

#include <vector>

#include "rendering/Renderer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "lights/Lights.h"
#include "lights/DirectionalLight.hpp"

class MeshRenderer : public Component, public Renderer
{
public:

	Mesh* mesh;
	Material* material;

	MeshRenderer() : Component(), Renderer()
	{
	}

	~MeshRenderer()
	{
	}

	void Render(Camera& camera) const
	{
		if (!gameObject) return;

		const Matrix4x4* projection = camera.ProjectionMatrix();
		const Matrix4x4* worldMatrix = gameObject->transform->LocalToWorldMatrix();
		const Matrix4x4* rotationMatrix = Matrix4x4::Rotate(*gameObject->transform->rotation);

		auto& lights = Lights::GetLights();
		
		if (material->enableTransparency)
		{
			*priority = 1;
			glEnable(GL_BLEND);
		}

		if (material->isTwoSided)
		{
			glDisable(GL_CULL_FACE);
		}

		material->Use();

		material->shader.SetMatrix4x4("transformMatrices.model", *worldMatrix);
		material->shader.SetMatrix4x4("transformMatrices.pv", *projection);
		material->shader.SetMatrix4x4("transformMatrices.rotation", *rotationMatrix);
		material->shader.SetVector("cameraPosition", *camera.transform->position);

		material->shader.SetColor("ambientColor", Color(0.3, 0.2, 0.1, 1));

		for (Light* light : lights)
		{
			switch (light->type)
			{
			case LightType::Directional:
				material->shader.SetColor("directionalLight.color", *light->color * light->intensity);
				material->shader.SetVector("directionalLight.direction", *(((DirectionalLight*)light)->direction));
				break;
			}
		}

		material->Activate();


		mesh->Draw();

		glDisable(GL_BLEND);
	}
};