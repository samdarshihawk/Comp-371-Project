#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "OBJloader.h"

class Model {
public:
	Model() = default;
	Model(GLuint VAO, GLuint numVertices, glm::vec3 color);
	virtual ~Model();

	virtual void draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation);

	virtual void drawTexture(const GLuint& TextureID, int ShaderProgram, glm::vec3 Scale, glm::vec3 Translate);

	virtual void setModelTransformMatrix(glm::mat4 modelTransformMatrix) { this->modelTransformMatrix = modelTransformMatrix; };
	void setColor(glm::vec3 color) { this->color = color; };

	glm::mat4 getModelTransformMatrix() { return this->modelTransformMatrix; };
	glm::vec3 getColor() { return this->color; };

	GLuint getVAO() { return VAO; }
	GLuint getNumVertices() { return numVertices; }

private:
	GLuint VAO;
	GLuint numVertices;
	glm::vec3 color;
	glm::mat4 modelTransformMatrix;
};
