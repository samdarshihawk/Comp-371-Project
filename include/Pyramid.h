#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h> 


using namespace glm;

struct TexturedConeColoredVertex
{
    TexturedConeColoredVertex(vec3 _position, vec3 _normal, vec2 _uv)
        : position(_position), normal(_normal), uv(_uv) {}

    vec3 position;
    vec3 normal;
    vec2 uv;
};

static const TexturedConeColoredVertex texturedConeVertexArray[] =
{
 TexturedConeColoredVertex(vec3(0.0f,0.5f,0.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f)), //left 
    TexturedConeColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f,-1.0f), vec2(1.0f, 0.0f)),
    TexturedConeColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.5f, 1.0f)),

    TexturedConeColoredVertex(vec3(0.0f,0.5f,0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedConeColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    TexturedConeColoredVertex(vec3(0.5f, -0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.5f, 1.0f)),

    TexturedConeColoredVertex(vec3(0.0f, 0.5f,0.0f), vec3(-1.0f, 0.0f,0.0f), vec2(0.0f, 1.0f)), // far
    TexturedConeColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f,0.0f), vec2(0.0f, 0.0f)),
    TexturedConeColoredVertex(vec3(-0.5f, -0.5f,-0.5f), vec3(-1.0f, 0.0f,0.0f), vec2(0.5f, 1.0f)),

    TexturedConeColoredVertex(vec3(0.0f, 0.5f,0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedConeColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedConeColoredVertex(vec3(-0.5f,-0.5f,0.5f), vec3(1.0f, 0.0f,0.0f), vec2(0.5f, 1.0f)),



};

unsigned int numOfVerticesTexturedCone = sizeof(texturedConeVertexArray) / sizeof(TexturedConeColoredVertex);



int createTexturedPyramidVertexArrayObject()
{
    // Create a vertex array
   
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedConeVertexArray), texturedConeVertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedConeColoredVertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedConeColoredVertex), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedConeColoredVertex), (void*)(2 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

   
    return vertexArrayObject;
}