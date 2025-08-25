#pragma once

#include <string>

#include <Moths/API.h>

#include <Moths/AssetManagement/Asset.h>
#include <Moths/AssetManagement/Data/Model.h>
#include <Moths/AssetManagement/ModelImporter.h>

#include <Moths/Entity/GameObject.h>
#include <Moths/Component/MeshRenderer.h>

namespace MothsEngine
{
	class MOTHS_API GameObjectAsset : public Asset
	{

	private:
		const Model* _model;
		GameObject* _gameObject;

	public:

		GameObject& GetGameObject()
		{
			return *_gameObject;
		}

		GameObjectAsset(const char* path) : Asset(path)
		{
		}

		~GameObjectAsset()
		{
			delete _model;
			delete _gameObject;
		}

		void Deserialize(char* bytes, size_t length)
		{
			_model = Internal::ModelImporter::Import(bytes, length);
			_gameObject = new GameObject();

			int jayneMeshSize = _model->meshes.size();
			for (int i = jayneMeshSize - 1; i >= 0; i--)
			{
				auto mesh = _model->meshes[i];
				MeshRenderer& renderer = _gameObject->AddComponent<MeshRenderer>();
				renderer.SetMesh(mesh);
				//renderer.animation = *_model->animations[0];
				renderer.armature = _model->armature;

				auto meshMaterials = mesh->GetMaterials();
				for (int i = 0; i < meshMaterials.size(); i++)
				{
					std::string name = meshMaterials[i].name;
					renderer.materials.push_back(_model->materials.at(name));
					Debug::Log("Mesh material name: " + name);
				}
			}
		}

		void ApplyMaterials(std::map<std::string, Material*> materials)
		{
			auto renderers = _gameObject->GetComponents<MeshRenderer>();
			for (auto renderer : renderers)
			{
				auto meshMaterials = renderer->GetMesh()->GetMaterials();

				for (auto meshMaterial : meshMaterials)
				{
					renderer->materials.push_back(materials[meshMaterial.name]);
				}
			}
		}

	};
}