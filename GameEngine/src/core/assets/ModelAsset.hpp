#pragma once
#include "../MeshContainer.hpp"
#include <map>
#include "../animation/Bone.h"
#include "ModelImporter.h"

using std::allocator;

class ModelAsset : public Asset
{
private:
	const Model* _model;

public:

	const Model* GetModel()
	{
		return _model;
	}

	ModelAsset(const char* path) : Asset(path)
	{

	}

	~ModelAsset()
	{
		delete _model;
	}

	void Deserialize(char* bytes, size_t length)
	{

		_model = ModelImporter::Import(bytes, length);
	}

};