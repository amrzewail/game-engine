#pragma once

using std::allocator;

class ModelAsset : public Asset
{
private:
	const aiScene* _scene;
	std::vector<Mesh*> _meshes;


	Mesh* ProcessMesh(aiMesh* mesh)
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

		std::cout << "Loaded mesh with vertices: " << vertices.size() << ", indices: " << indices.size() << std::endl;

		return new Mesh(vertices, indices);

		//std::vector<Vertex> obj_pts;

		//Vertex v1;
		//v1.position = Vector(.5f, -.5f, 0.f);
		//obj_pts.push_back(v1);

		//Vertex v2;
		//v2.position = Vector(-.5f, -.5f, 0.f);
		//obj_pts.push_back(v2);

		//Vertex v3;
		//v3.position = Vector(0.f, .5f, 0.f);
		//obj_pts.push_back(v3);

		//std::vector<uint32_t> elem = { 0, 1, 2 };

		//return new Mesh(obj_pts, elem);


	}

	void ProcessNode(aiNode* node)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aMesh = _scene->mMeshes[node->mMeshes[i]];
			Mesh* mesh = ProcessMesh(aMesh);
			_meshes.push_back(mesh);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i]);
		}
	}



public:

	std::vector<Mesh*> Meshes()
	{
		return _meshes;
	}

	~ModelAsset()
	{
		delete _scene;
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

	}

};