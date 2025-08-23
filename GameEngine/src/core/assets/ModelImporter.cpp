#include "ModelImporter.h"

#include "importers/AssimpImporter.hpp"

Model* ModelImporter::Import(char* bytes, size_t length)
{
	AssimpImporter importer;
	auto model = importer.Import(bytes, length);
	importer.Release();
	return model;
}


void ModelImporter::Release(Model* model)
{
	delete model;
}