#pragma once

#include <vector>
#include <glad/glad.h>

#include "Vertex.h"

struct Mesh
{
private:
	mutable bool verticesDirty = false;
	uint32_t VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void SetupMesh();

public:


	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	std::vector<Vertex> Vertices();
	std::vector<unsigned int> Indices();

	void Draw() const;
};