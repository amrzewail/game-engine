#pragma once


#include <Moths/AssetManagement/Data/Model.h>
#include <Moths/AssetManagement/Importers/AssimpImporter.h>

namespace MothsEngine::Internal
{

	class ModelImporter
	{
	public:

		inline static Model* Import(char* bytes, size_t length)
		{
			AssimpImporter importer;
			auto model = importer.Import(bytes, length);
			importer.Release();
			return model;
		}

		inline static void Release(Model* model)
		{
			delete model;
		}
	};

}