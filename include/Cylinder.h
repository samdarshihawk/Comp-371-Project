#pragma once

#include <iostream>
#include <list>
#include <vector>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace glm;
using namespace std;


GLuint CylinderVAO;
GLuint CylinderVBO;
GLuint ConeVAO;
GLuint ConeVBO;

struct Cylinder
{
    Cylinder(vec3 _vertices, vec3 _normals, vec2 _uv)
        : cylinderVertices(_vertices), cylinderNormals(_normals), cylinderUVs(_uv) {}
    Cylinder() {};

    vec3 cylinderVertices;
    vec3 cylinderNormals;
    vec2 cylinderUVs;

    void print() {
        std::cout << cylinderVertices.x << ", " << cylinderVertices.y << ", " << cylinderVertices.z << std::endl;
    }
};

std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
std::vector<glm::vec3> normals = std::vector<glm::vec3>();
std::vector<glm::vec2> uv = std::vector<glm::vec2>();
std::vector<unsigned int> Coneindices = std::vector<unsigned int>();
std::vector<unsigned int> Cylinderindices = std::vector<unsigned int>();

vector<Cylinder> GenerateCylinder(int baseRadius, int topRadius, int height, int sectorCount, vector<unsigned int>& indices)
{
    std::vector<Cylinder> cylinders = std::vector<Cylinder>();
    float x, y, z;                                  // vertex position
    float radius;                                   // radius for each stack

    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> sideNormals;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        sideNormals.push_back(cos(sectorAngle) * x0 - sin(sectorAngle) * y0);   // nx
        sideNormals.push_back(sin(sectorAngle) * x0 + cos(sectorAngle) * y0);   // ny
        sideNormals.push_back(z0);  // nz
    }

    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    unsigned int baseVertexIndex = (unsigned int)vertices.size();

    z = -height * 0.5f;
    vertices.push_back(glm::vec3(0, 0, z));
    normals.push_back(glm::vec3(0, 0, -1));
    uv.push_back(glm::vec2(0.5f, 0.5f));
    for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        vertices.push_back(glm::vec3(x * baseRadius, y * baseRadius, z));
        normals.push_back(glm::vec3(0, 0, -1));
        uv.push_back(glm::vec2(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f));
    }

    unsigned int topVertexIndex = (unsigned int)vertices.size();
    // put vertices of top of cylinder
    z = height * 0.5f;
    vertices.push_back(glm::vec3(0, 0, z));
    normals.push_back(glm::vec3(0, 0, 1));
    uv.push_back(glm::vec2(0.5f, 0.5f));
    for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        vertices.push_back(glm::vec3(x * topRadius, y * topRadius, z));
        normals.push_back(glm::vec3(0, 0, 1));
        uv.push_back(glm::vec2(x * 0.5f + 0.5f, -y * 0.5f + 0.5f));
    }

    int k1 = 0;                         // 1st vertex index at base
    int k2 = sectorCount + 1;           // 1st vertex index at top

    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
    {
        if (j == 0)
        {
            // 2 trianles per sector
            indices.push_back(k1 + sectorCount);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + sectorCount);

            indices.push_back(k2 + sectorCount);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }
        else
        {
            // 2 trianles per sector
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }
    }

    // put indices for base
    for (int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < (sectorCount - 1))
        {
            indices.push_back(baseVertexIndex);
            indices.push_back(k + 1);
            indices.push_back(k);
        }
        else    // last triangle
        {
            indices.push_back(baseVertexIndex);
            indices.push_back(baseVertexIndex + 1);
            indices.push_back(k);
        }
    }

    // put indices for top
    for (int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < (sectorCount - 1))
        {
            indices.push_back(topVertexIndex);
            indices.push_back(k);
            indices.push_back(k + 1);
        }
        else
        {
            indices.push_back(topVertexIndex);
            indices.push_back(k);
            indices.push_back(topVertexIndex + 1);
        }
    }

    int sizeofitall = (vertices.size() + normals.size() + uv.size());

    for (int i = 0; i < vertices.size(); i++)
    {
        cylinders.push_back(Cylinder(vertices.at(i), normals.at(i), uv.at(i)));
    }

    cout << "Indices: " << indices.size() << endl;
    cout << "Vertices: " << vertices.size() << endl;

    vertices.clear();
    normals.clear();
    uv.clear();



    return cylinders;
}

GLuint createCylinderVAO() //add boolean to initiate array differently (120-100)
{
    cout << "Create Cylinder Begins: " << endl;
    std::vector<Cylinder> cylinders = GenerateCylinder(4, 4, 10, 10, Cylinderindices);

    static Cylinder cylindertopass[120];

    for (int i = 0; i < Cylinderindices.size(); i++) {
        cylindertopass[i] = cylinders.at(Cylinderindices[i]);
    }

    // static const Cylinder cylindertopass = *cylinder;
    glGenVertexArrays(1, &CylinderVAO);
    glBindVertexArray(CylinderVAO);

    glGenBuffers(1, &CylinderVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylindertopass), cylindertopass, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)(2 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)(3 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

    return CylinderVAO;
}

GLuint createConeVAO() //add boolean to initiate array differently (120-100)
{
    cout << "Create Cone Begins: " << endl;
    std::vector<Cylinder> cylinders = GenerateCylinder(4, 0, 10, 10, Coneindices);

    static Cylinder conetopass[120];

    for (int i = 0; i < Coneindices.size(); i++) {
        conetopass[i] = cylinders.at(Coneindices[i]);
    }

    // static const Cylinder cylindertopass = *cylinder;
    glGenVertexArrays(1, &ConeVAO);
    glBindVertexArray(ConeVAO);

    glGenBuffers(1, &ConeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ConeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(conetopass), conetopass, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Cylinder), (void*)(2 * sizeof(vec3)));
    glEnableVertexAttribArray(2);

    return ConeVAO;
}


//--------------EXAMPLE FOR RENDER -----------------------------//
/*void renderCylinder(const GLuint& cylinderShader, GLuint VAO)
{
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
    glUseProgram(cylinderShader);
    cout << "In render function Cylinder: " << VAO << endl;
    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(glGetUniformLocation(cylinderShader, "withText"), 0);
    glFrontFace(GL_CW);
    mat4 cylinderMatrix = translateSky * scaleSky;
    GLuint MVP_Cylinder = glGetUniformLocation(cylinderShader, "mvp");
    GLuint Color_Cylinder = glGetUniformLocation(cylinderShader, "color");
    mat4 Cyl;
    if (!textureOn) {
        Cyl = cylinderMatrix * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f));
        glUniformMatrix4fv(MVP_Cylinder, 1, GL_FALSE, &Cyl[0][0]);
        glUniform3f(Color_Cylinder, 1.0f, 1.0f, 1.0f);
        glDrawArrays(primativeRender1, 0, Cylinderindices.size());
    }
    glFrontFace(GL_CW);
    glUniform1ui(glGetUniformLocation(cylinderShader, "withText"), 1);
    if (textureOn) {
        Cyl = cylinderMatrix * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f));
        glUniformMatrix4fv(MVP_Cylinder, 1, GL_FALSE, &Cyl[0][0]);
        glBindTexture(GL_TEXTURE_2D, wheelsTextureId);
        glDrawArrays(primativeRender1, 0, Cylinderindices.size());
    }
    glBindVertexArray(0);
}*/