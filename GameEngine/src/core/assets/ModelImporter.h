#pragma once


#include "data/Model.hpp"

class ModelImporter
{


public:

	static Model* Import(char* bytes, size_t length);

	static void Release(Model* model);
};

