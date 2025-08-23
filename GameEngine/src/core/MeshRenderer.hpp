#pragma once

#include <vector>

#include "rendering/Renderer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Submesh.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "lights/Lights.h"
#include "lights/DirectionalLight.hpp"
#include "Screen.h"

#include "animation/Animation.hpp"
#include "animation/Armature.hpp"

#define VERTEX_LOCATION 0
#define NORMAL_LOCATION 1
#define UVS_LOCATION 2
#define BONE_ID_LOCATION 3
#define BONE_WEIGHT_LOCATION 4


class MeshRenderer : public Renderer
{
private:
	bool _didSetMesh = false;
	mutable bool verticesDirty = false;
	std::vector<uint32_t> VAOs;
	std::vector<uint32_t*> VBOs;
	std::vector<uint32_t> EBOs;

	Mesh* _mesh;

	void Clear()
	{
		for (int i = 0; i < VAOs.size(); i++)
		{
			glDeleteBuffers(4, VBOs[i]);
			glDeleteBuffers(1, &EBOs[i]);
			glDeleteVertexArrays(1, &VAOs[i]);
		}

		VAOs.clear();
		VBOs.clear();
		EBOs.clear();
	}

	void SetupMesh()
	{
		if (_didSetMesh)
		{
			Clear();
		}

		std::vector<Submesh*> submeshes = _mesh->GetSubmeshes();

		for (auto submesh : submeshes)
		{
			uint32_t vao, ebo;
			uint32_t vbos[4];

			verticesDirty = true;

			// Vertex Array Object
			// Vertex Buffer Object

			glGenVertexArrays(1, &vao);
			glGenBuffers(4, vbos);
			glGenBuffers(1, &ebo);


			auto indices = submesh->indices;
			auto vertices = submesh->vertices;
			auto normals = submesh->normals;
			auto uvs = submesh->uvs;
			auto weights = submesh->boneWeights;

			glBindVertexArray(vao);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			// vertex positions
			glBindBuffer(GL_ARRAY_BUFFER, vbos[VERTEX_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector), vertices.data(), GL_STREAM_DRAW);
			glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector), 0);
			glEnableVertexAttribArray(VERTEX_LOCATION);

			//// vertex normals
			glBindBuffer(GL_ARRAY_BUFFER, vbos[NORMAL_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector), normals.data(), GL_STREAM_DRAW);
			glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector), 0);
			glEnableVertexAttribArray(NORMAL_LOCATION);

			// vertex texture coords
			glBindBuffer(GL_ARRAY_BUFFER, vbos[UVS_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), uvs.data(), GL_STREAM_DRAW);
			glVertexAttribPointer(UVS_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);
			glEnableVertexAttribArray(UVS_LOCATION);

			//bone ids
			glBindBuffer(GL_ARRAY_BUFFER, vbos[BONE_ID_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(BoneWeight), weights.data(), GL_STREAM_DRAW);

			glVertexAttribIPointer(BONE_ID_LOCATION, BoneWeight::NUM_BONES_PER_VERTEX, GL_INT, sizeof(BoneWeight), (const GLvoid*)0);
			glEnableVertexAttribArray(BONE_ID_LOCATION);

			//bone weights
			glVertexAttribPointer(BONE_WEIGHT_LOCATION, BoneWeight::NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(BoneWeight), (const GLvoid*)(BoneWeight::NUM_BONES_PER_VERTEX * sizeof(int)));
			glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			VAOs.push_back(vao);
			VBOs.push_back(vbos);
			EBOs.push_back(ebo);
		}

	}

public:

	Animation animation;
	Armature* armature;

	MeshRenderer() : Renderer()
	{
	}

	~MeshRenderer()
	{
		if (_didSetMesh)
		{
			Clear();
		}
	}

	void SetMesh(Mesh* mesh)
	{
		_didSetMesh = false;

		_mesh = mesh;

		SetupMesh();

		if (mesh == nullptr) return;

		_didSetMesh = true;
	}

	void Render(Camera& camera)
	{
		Renderer::Render(camera);
	}

	inline void UpdateBones(int submeshIndex)
	{
		Keyframe* keyframe = animation.GetKeyframe(0);

		auto keyframeProperties = keyframe->GetProperties();

		for (auto& property : keyframeProperties)
		{
			auto bone = armature->GetBoneByName(property.first);
			if (!bone) continue;
			bone->localTransform = property.second->TRS;
			bone->globalTransform = bone->localTransform;
		}

		for (auto bone : armature->GetBones())
		{
			if (bone->parentIndex == -1)
			{
				bone->globalTransform = bone->localTransform;
			}
			else
			{
				auto parent = armature->GetBone(bone->parentIndex);
				bone->globalTransform = parent->globalTransform * bone->localTransform;
			}
		}
	}

	inline void PreDraw(Camera& camera, Material* material, int submeshIndex)
	{
		UpdateBones(submeshIndex);

		Matrix4x4 finalTransforms[128];
		for (int i = 0; i < 128; i++) finalTransforms[i] = Matrix4x4::Identity();

		auto submesh = _mesh->GetSubmeshes()[submeshIndex];
		for (auto bone : submesh->bones)
		{
			auto armatureBone = armature->GetBoneByName(bone.name);
			if (!armatureBone) continue;
			finalTransforms[armatureBone->index] = armature->GlobaInverseTransform() * armatureBone->globalTransform * bone.inverseBindMatrix;
		}

		material->shader.SetMatrix4x4Array("boneMatrices", finalTransforms, 128);
	}


	void Draw(Camera& camera, int index)
	{
		if (!_didSetMesh) return;

		if (index >= VAOs.size()) return;

		auto submesh = _mesh->GetSubmeshes()[index];

		glBindVertexArray(VAOs[index]);

		auto indices = submesh->indices;
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}
};