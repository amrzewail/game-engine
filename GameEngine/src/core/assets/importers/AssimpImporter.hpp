#pragma once

#include "../data/Model.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include "../../Mesh.hpp"
#include "../../animation/Bone.h"

#include <unordered_map>
#include <set>
#include <stack>

using namespace std;

class AssimpImporter
{

private:
	const aiScene* _scene;

	std::string GetAiMeshMaterialName(aiMesh* mesh)
	{
		aiMaterial* material = _scene->mMaterials[mesh->mMaterialIndex];
		aiString materialName;
		aiReturn ret;
		ret = material->Get(AI_MATKEY_NAME, materialName);
		if (ret != AI_SUCCESS) materialName = "";
		return materialName.C_Str();
	}

	inline Submesh* ProcessAiMesh(aiMesh* mesh, Model& model, Matrix4x4 correction)
	{
		Submesh* submesh = new Submesh();

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			{ // Position
				Vector vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				submesh->vertices.push_back(correction * vector);
			}
			{ // Normal
				Vector vector;
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				submesh->normals.push_back(correction * vector);

			}
			{ //TexCoords
				Vector2 vector(0, 0);
				if (mesh->mTextureCoords[0])
				{
					vector.x = mesh->mTextureCoords[0][i].x;
					vector.y = mesh->mTextureCoords[0][i].y;
				}
				submesh->uvs.push_back(vector);
			}
			{ // Populate empty Bone Weights
				BoneWeight weight;
				weight.ids[0] = 0;
				weight.ids[1] = 0;
				weight.ids[2] = 0;
				weight.ids[3] = 0;
				weight.weights[0] = 0;
				weight.weights[1] = 0;
				weight.weights[2] = 0;
				weight.weights[3] = 0;
				submesh->boneWeights.push_back(weight);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			submesh->indices.push_back(face.mIndices[0]);
			submesh->indices.push_back(face.mIndices[2]);
			submesh->indices.push_back(face.mIndices[1]);
		}

		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			MeshBone bone;
			bone.name = mesh->mBones[i]->mName.C_Str();
			bone.inverseBindMatrix = FromAiMatrix(mesh->mBones[i]->mOffsetMatrix);
			submesh->bones.push_back(bone);

			int boneIndex = model.armature->GetBoneByName(bone.name)->index;

			for (int w = 0; w < mesh->mBones[i]->mNumWeights; w++)
			{
				auto mWeight = mesh->mBones[i]->mWeights[w];
				BoneWeight weight = submesh->boneWeights[mWeight.mVertexId];
				weight.AddBoneData(boneIndex, mWeight.mWeight);
				submesh->boneWeights[mWeight.mVertexId] = weight;
			}
		}

		return submesh;
	}

	void ProcessArmature(Model* model, aiNode* root)
	{
		std::set<std::string> boneNames;
		std::stack<aiNode*> nodes;
		nodes.push(root);
		while (!nodes.empty())
		{
			aiNode* n = nodes.top();
			nodes.pop();

			for (unsigned int i = 0; i < n->mNumMeshes; i++)
			{
				aiMesh* mesh = _scene->mMeshes[n->mMeshes[i]];
				if (!mesh->HasBones()) continue;
				for (unsigned int j = 0; j < mesh->mNumBones; j++) boneNames.emplace(mesh->mBones[j]->mName.C_Str());
			}

			for (unsigned int i = 0; i < n->mNumChildren; i++) nodes.push(n->mChildren[i]);
		}

		*model->armature = Armature(model->globalInverseTransform);

		std::stack<std::pair<const aiNode*, int>> bones;
		bones.push({ root, -1 });
		while (!bones.empty())
		{
			auto [node, parentIndex] = bones.top();
			bones.pop();

			int boneIndex = -1;

			std::string name(node->mName.C_Str());
			if (boneNames.find(name) != boneNames.end())
			{
				Bone bone;
				bone.name = name;
				bone.parentIndex = parentIndex;
				bone.localTransform = FromAiMatrix(node->mTransformation);
				boneIndex = model->armature->AddBone(name, bone);
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++) bones.push({ node->mChildren[i], boneIndex });
		}
	}


	void ProcessNode(Model* model, aiNode* node, std::vector<Mesh*>* meshes, Matrix4x4 correction)
	{
		if (node->mNumMeshes > 0)
		{
			Mesh* mesh = new Mesh();

			std::cout << "Loading node: " << node->mName.C_Str() << std::endl;

			int numVertices = 0;
			int numIndices = 0;

			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* aMesh = _scene->mMeshes[node->mMeshes[i]];
				if (aMesh->mNumVertices == 0) continue;

				Submesh* submesh = ProcessAiMesh(aMesh, *model, correction);

				mesh->AddSubmesh(submesh);
				mesh->AddMaterialName(GetAiMeshMaterialName(aMesh));

				submesh->meshVertexIndex = numVertices;
				submesh->meshIndiceIndex = numIndices;
				submesh->materialIndex = i;

				numVertices += submesh->vertices.size();
				numIndices += submesh->indices.size();
			}

			std::cout << std::endl;
			meshes->push_back(mesh);
		}


		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(model, node->mChildren[i], meshes, correction);
		}
	}

	void ProcessAnimation(aiAnimation& aAnimation, Animation* animation)
	{
		animation->SetName(aAnimation.mName.C_Str());
		animation->SetTicksPerSecond(aAnimation.mTicksPerSecond > 0 ? aAnimation.mTicksPerSecond : 1);
		animation->SetDuration(aAnimation.mTicksPerSecond > 0 ? aAnimation.mDuration / aAnimation.mTicksPerSecond : 1);

		//load positions rotations and scales for each bone
	// each channel represents each bone
		for (int i = 0; i < aAnimation.mNumChannels; i++)
		{
			aiNodeAnim* channel = aAnimation.mChannels[i];

			std::string boneName = channel->mNodeName.C_Str();

			for (int j = 0; j < channel->mNumPositionKeys; j++)
			{
				Keyframe* keyframe = animation->GetKeyframe(channel->mPositionKeys[j].mTime);
				auto p = KeyframeProperty();
				p.TRS.Translate(Vector(channel->mPositionKeys[j].mValue.x, channel->mPositionKeys[j].mValue.y, channel->mPositionKeys[j].mValue.z));
				keyframe->AddProperty(boneName, p);
				//std::cout << "ANIMATION POSTIION NAME: " << boneName << " TIME: " << keyframe->Tick() << " VALUE: " << p.position.ToString() << std::endl;
			}

			for (int j = 0; j < channel->mNumRotationKeys; j++)
			{
				Keyframe* keyframe = animation->GetKeyframe(channel->mRotationKeys[j].mTime);
				auto p = KeyframeProperty();
				p.TRS.Rotate(Quaternion(channel->mRotationKeys[j].mValue.w, channel->mRotationKeys[j].mValue.x, channel->mRotationKeys[j].mValue.y, channel->mRotationKeys[j].mValue.z));
				keyframe->AddProperty(boneName, p);
				//std::cout << "ANIMATION ROTATION NAME: " << boneName << " TIME: " << keyframe->Tick() << " VALUE: " << p.rotation.EulerAngles().ToString() << std::endl;
			}
		}
	}

	void ProcessAnimations(Model* model, const aiScene& aScene)
	{
		if (!aScene.HasAnimations()) return;

		for (int i = 0; i < aScene.mNumAnimations; i++)
		{
			auto aAnimation = aScene.mAnimations[i];

			Animation* animation = new Animation();

			ProcessAnimation(*aAnimation, animation);

			model->animations.push_back(animation);
		}
	}

public:

	Model* Import(char* bytes, size_t length)
	{
		Model* model = new Model();

		Assimp::Importer importer;

		_scene = importer.ReadFileFromMemory(bytes, length, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PopulateArmatureData);

		if (_scene == nullptr)
		{
			return model;
		}

		// correction for exports of Y forward and Z upward
		Matrix4x4 correction = Matrix4x4::TRS(Vector::Zero(), Quaternion::Euler(-90, 0, 0), Vector::One());

		model->globalInverseTransform = FromAiMatrix(_scene->mRootNode->mTransformation);

		model->globalInverseTransform = correction * model->globalInverseTransform;
		model->globalInverseTransform.SetInverse();

		ProcessArmature(model, _scene->mRootNode);
		ProcessNode(model, _scene->mRootNode, &model->meshes, correction);
		ProcessAnimations(model, *_scene);

		importer.FreeScene();

		return model;
	}



	Matrix4x4 FromAiMatrix(aiMatrix4x4 matrix)
	{
		float a1 = matrix.a1, a2 = matrix.a2, a3 = matrix.a3, a4 = matrix.a4;
		float b1 = matrix.b1, b2 = matrix.b2, b3 = matrix.b3, b4 = matrix.b4;
		float c1 = matrix.c1, c2 = matrix.c3, c3 = matrix.c3, c4 = matrix.c4;
		float d1 = matrix.d1, d2 = matrix.d2, d3 = matrix.d3, d4 = matrix.d4;

		Matrix4x4 mat;
		mat.Set(a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4);
		return mat;
	}


	void Release()
	{

	}

};