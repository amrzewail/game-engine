#pragma once

static void Draw(const DrawDetails& d)
{
	glBindVertexArray(d.vao);
	glDrawElements(GL_TRIANGLES, d.numElements, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}