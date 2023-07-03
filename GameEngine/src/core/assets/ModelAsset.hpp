#pragma once
#include "../MeshContainer.hpp"

using std::allocator;

class ModelAsset : public Asset
{
private:
	const aiScene* _scene;
	std::vector<MeshContainer*> _meshes;


	MeshContainer* ProcessMesh(aiMesh* mesh)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			{ // Position
				Vector vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;
			}
			{ // Normal
				Vector vector;
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}
			if (mesh->mTextureCoords[0])
			{
				Vector2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
			{
				vertex.texCoords = Vector2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				const unsigned int vertexIndex = face.mIndices[j];
				if (vertexIndex >= vertices.size())
				{
					std::cerr << "Unexpected element index, " << vertexIndex
						<< " vs vertex count: " << vertices.size() << std::endl;
					exit(1);
				}
				indices.push_back(vertexIndex);
			}
		}

		MeshContainer* container = new MeshContainer();

		Mesh* m = new Mesh(vertices, indices);

		container->mesh = m;

		if (_scene->HasMaterials())
		{
			aiMaterial* material = _scene->mMaterials[mesh->mMaterialIndex];
			aiString materialName;
			aiReturn ret;
			ret = material->Get(AI_MATKEY_NAME, materialName);
			if (ret != AI_SUCCESS) materialName = "";
			*container->materialName = materialName.C_Str();
			*container->materialIndex = mesh->mMaterialIndex;
		}


		std::cout << "Loaded mesh with material: " << *container->materialName << " with vertices: " << vertices.size() << ", indices: " << indices.size() << std::endl;

		return container;
	}

	void ProcessNode(aiNode* node)
	{
		if (node->mNumMeshes > 0)
		{
			std::cout << "Loading node: " << node->mName.C_Str() << std::endl;

			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* aMesh = _scene->mMeshes[node->mMeshes[i]];

				MeshContainer* container = ProcessMesh(aMesh);

				_meshes.push_back(container);

			}

			std::cout << std::endl;
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i]);
		}
	}



public:

	std::vector<MeshContainer*> Meshes()
	{
		return _meshes;
	}

	ModelAsset(const char* path) : Asset(path)
	{

	}

	~ModelAsset()
	{
		for (int i = 0; i < _meshes.size(); ++i)
		{
			delete _meshes[i];
		}
	}

	void Deserialize(char* bytes, size_t length)
	{
		Assimp::Importer importer;

		_scene = importer.ReadFileFromMemory(bytes, length, aiProcess_Triangulate | aiProcess_FlipUVs);


		if (_scene == nullptr)
		{
			return;
		}

		ProcessNode(_scene->mRootNode);


		importer.FreeScene();
	}

};