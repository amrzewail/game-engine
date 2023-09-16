#pragma once

#include <vector>

#include "rendering/Renderer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "lights/Lights.h"
#include "lights/DirectionalLight.hpp"
#include "Screen.h"

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

		const Matrix4x4& projection = camera.ProjectionMatrix();
		const Matrix4x4& worldMatrix = gameObject->transform->LocalToWorldMatrix();
		const Matrix4x4* rotationMatrix = Matrix4x4::Rotate(*gameObject->transform->rotation);
		const Matrix4x4& cameraPositionMatrix = camera.transform->PositionMatrix();

		auto& lights = Lights::GetLights();
		
		if (material->shader.IsDefined(Shader::FLAG_TRANSPARENT))
		{
			*priority = 10;
		}

		if (material->shader.IsDefined(Shader::FLAG_DEPTH_TEST_OFF))
		{
			glDisable(GL_DEPTH_TEST);
			glDepthMask(false);
		}

		if (material->shader.IsDefined(Shader::FLAG_Blend_SrcAlpha_OneMinusSrcAlpha))
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		if (material->isTwoSided || material->shader.IsDefined(Shader::FLAG_CULL_FACE_OFF))
		{
			glDisable(GL_CULL_FACE);
		}

		material->Use();

		material->shader.SetMatrix4x4("transformMatrices.model", worldMatrix);
		material->shader.SetMatrix4x4("transformMatrices.pv", projection);
		material->shader.SetMatrix4x4("transformMatrices.rotation", *rotationMatrix);

		Vector cameraPosition = *camera.transform->position;
		Vector cameraForward = camera.transform->Forward();
		material->shader.SetVector("cameraPosition", cameraPosition);
		material->shader.SetVector("cameraDirection", cameraForward);

		material->shader.SetColor("ambientColor", Color(0.3, 0.2, 0.1, 1));

		material->shader.SetVector2("screenResolution", Vector2(Screen::WIDTH, Screen::HEIGHT));

		for (Light* light : lights)
		{
			switch (light->type)
			{
			case LightType::Directional:
				material->shader.SetColor("directionalLight.color", *light->color * light->intensity);
				Vector directionalLightDir = *(((DirectionalLight*)light)->direction);
				directionalLightDir.z *= -1;
				material->shader.SetVector("directionalLight.direction", directionalLightDir);
				break;
			}
		}

		material->Activate();


		mesh->Draw();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
};