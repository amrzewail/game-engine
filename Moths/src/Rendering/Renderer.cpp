#include <Moths/Component/Component.h>
#include <Moths/Component/Renderer.h>
#include <Moths/Rendering/Renderers.h>

#include <Moths/Entity/GameObject.h>
#include <Moths/Lighting/Lights.h>
#include <Moths/Lighting/DirectionalLight.h>

#include <Moths/Core/Shader.h>
#include <Moths/Core/Screen.h>

using namespace MothsEngine;
using namespace MothsEngine::Internal;

Renderer::~Renderer()
{
	Renderers::RemoveRenderer(this);

	delete priority;
}

Renderer::Renderer() : Component()
{
	*priority = 0;
	Renderers::AddRenderer(this);
}


void Renderer::Render(Camera& camera)
{
	if (!gameObject) return;

	const Matrix4x4 projection = camera.ProjectionMatrix();
	const Matrix4x4 worldMatrix = gameObject->transform->LocalToWorldMatrix();
	const Matrix4x4 rotationMatrix = Matrix4x4::Rotate(gameObject->transform->get_Rotation());
	const Matrix4x4 cameraPositionMatrix = camera.transform->PositionMatrix();

	auto& lights = Lights::GetLights();

	for (int i = 0; i < materials.size(); i++)
	{
		auto material = materials[i];

		if (!material) continue;

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
		material->shader.SetMatrix4x4("transformMatrices.rotation", rotationMatrix);

		Vector cameraPosition = camera.transform->position;
		Vector cameraForward = camera.transform->Forward();
		material->shader.SetVector("cameraPosition", cameraPosition);
		material->shader.SetVector("cameraDirection", cameraForward);

		material->shader.SetColor("ambientColor", Color(0.1, 0.1, 0.1, 1));

		material->shader.SetVector2("screenResolution", Vector2(Screen::Width(), Screen::Height()));

		for (Light* light : lights)
		{
			switch (light->type)
			{
			case LightType::Directional:
				material->shader.SetColor("directionalLight.color", light->color);
				material->shader.SetFloat("directionalLight.intensity", light->intensity);
				Vector directionalLightDir = (((DirectionalLight*)light)->direction);
				material->shader.SetVector("directionalLight.direction", directionalLightDir);
				break;
			}
		}

		//PreDraw(camera, material, i);

		material->Activate();

		Draw(camera, i);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}

void Renderer::SetMaterial(Material* material)
{
	materials[0] = material;
}

