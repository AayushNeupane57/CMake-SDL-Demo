#include "renderer.h"

void Renderer::DrawVertices(GLuint vaoId, GLenum mode, int vertexCount) {
	glBindVertexArray(vaoId);
	glDrawArrays(mode, 0, vertexCount);
	glBindVertexArray(0);
}
