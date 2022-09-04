#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h> 

using namespace glm;

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _normal, vec2 _uv, vec3 _color)
        : position(_position), normal(_normal), uv(_uv), color(_color) {}

    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 color;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {                                                                // position,                            color
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)), // left - red
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f)),

    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)), // top - yellow
    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f))
    
};


int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                             // attribute 0 matches aPos in Vertex Shader
                          3,                             // size
                          GL_FLOAT,                      // type
                          GL_FALSE,                      // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void *)0                      // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)sizeof(vec3) // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)(2 * sizeof(vec3)) // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, // attribute 2 matches aUV in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)(3 * sizeof(vec3)) // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(3);

    

    return vertexArrayObject;
}