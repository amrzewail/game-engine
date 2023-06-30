#pragma once

static DrawDetails UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t> elem)
{
	if (verts.empty() || elem.empty())
	{
		throw("empty vector");
	}

	uint32_t VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);


	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return DrawDetails(VAO, elem.size());
}


static void UnloadMesh(std::vector<DrawDetails>& details)
{
	for (auto& d : details)
	{
		glDeleteBuffers(1, &d.vao);
	}
	details.clear();
}