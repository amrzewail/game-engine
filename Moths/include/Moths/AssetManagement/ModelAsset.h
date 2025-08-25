#pragma once

#include <map>

#include <Moths/API.h>
#include <Moths/AssetManagement/Asset.h>
#include <Moths/AssetManagement/ModelImporter.h>
#include <Moths/AssetManagement/Data/Model.h>
#include <Moths/Animation/Bone.h>

namespace MothsEngine
{

	using std::allocator;

	class MOTHS_API ModelAsset : public Asset
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

			_model = Internal::ModelImporter::Import(bytes, length);
		}

	};
}