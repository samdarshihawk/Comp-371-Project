#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <list>
#include <stb_image.h>
#include <time.h>

#include "generateCube.h"
#include "Pyramid.h"
#include "shaderloader.h"
#include "createWindow.h"
#include "loadTexture.h"
#include "PerlinNoise.hpp"
#include "Tree.h"
#include "Cylinder.h"
#include "OBJloader.h"
#include "Assets.h"
#include "ObjModel.h"
#include "PerlinNoise.hpp"

using namespace glm;
using namespace std;

// ---------- Global Parameters ---------- //
// ----- Window Parameters -----
int windowWidth = 1024;
int windowHeight = 768;
char windowTitle[] = "Comp-371 | Project"; 

// ----- Shader Preferences -----
std::string shaderPathPrefix = "assets/shaders/";

GLuint shaderScene;
GLuint shaderShadow;
GLuint shaderHeightMap;

// ----- Projection, View & Model matrix -----
mat4 projectionMatrix;
mat4 viewMatrix;
mat4 modelMatrix;
mat4 terrainModelMatrix;

// --------- Load Textures  ----------
GLuint blueTexture;
GLuint redTexture;
GLuint pineTexture;
GLuint barkTexture;
GLuint StoneTexture;
GLuint BrickTexture;
GLuint RoofTexture;
GLuint HouseWallTexture;
GLuint grassTexture;
GLuint PyramidTexture;
GLuint SandTexture;
GLuint towerTopTexture;
GLuint stoneWall;
GLuint mushroomTexture;

//------------ Load Assets-------------
Model stone;
Model bench;
Model grass1;
Model grass2;
Model grass3;
int cube_vao;
int pyramid_vao;
int cylinder_vao;
int cone_vao;


// ----- Perlin Noise Settings -----
const siv::PerlinNoise::seed_type seed = 63574u;
const siv::PerlinNoise perlin{ seed };
vector< siv::PerlinNoise::seed_type> seeds;

const int noise_width = 500;
const int noise_height = 500;

vector<float> noise_vertices;
float noise_yScale = 64.0f / 256.0f;
float noise_yShift = 5.0f;
int noise_rez = 1;
float noise_y;
int numStrips = 0;
int numTrisPerStrip = 0;
vec3 terrainSize = vec3(0.0f, -1.0f, 0.0f);


// ----- VAOs -----
unsigned int terrainVAO, terrainVBO, terrainIBO;


// ----- Terrain -----
vector<vec3> terrainArray; 

vector<unsigned int> vaoArray;
vector<int> vaoIndexArray;

float terr_mult_1 = 1.0f;
float terr_mult_2 = 1.0f;
float terr_mult_3 = 1.0f;
float terr_mult_4 = 1.0f;
float terr_mult_5 = 1.0f;
float terr_mult_6 = 1.0f;
float terr_mult_7 = 1.0f;
float terr_mult_8 = 1.0f;
float terr_mult_9 = 1.0f;
float terr_mult_10 = 1.0f;
float terr_mult_11 = 1.0f;
float terr_mult_12 = 1.0f;


vec3 lightPosition;


// shader variable setters
void SetUniformMat4(GLuint shader_id, const char* uniform_name, mat4 uniform_value)
{
  glUseProgram(shader_id);
  glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &uniform_value[0][0]);
}

void SetUniformVec3(GLuint shader_id, const char* uniform_name, vec3 uniform_value)
{
  glUseProgram(shader_id);
  glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, value_ptr(uniform_value));
}

template <class T>
void SetUniform1Value(GLuint shader_id, const char* uniform_name, T uniform_value)
{
  glUseProgram(shader_id);
  glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
  glUseProgram(0);
}


/**
 * @brief Set the Projection Matrix object
 * 
 * @param shaderProgram - Name of the shader to use
 * @param projectionMatrix - Projection Matrix
 */
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projection_matrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

/**
 * @brief Set the View Matrix object
 * 
 * @param shaderProgram - Name of the shader to use
 * @param viewMatrix - View Matrix
 */
void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "view_matrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

/**
 * @brief Set the World Matrix object
 * 
 * @param shaderProgram - Name of the shader to use
 * @param worldMatrix - Projection Matrix
 */
void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "model_matrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setTerrainMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}




float noise_values[noise_width][noise_height];

void generateNoise(float array[][noise_width], siv::PerlinNoise perlin1) {
    
   int y = 5;

    for (int y = 0; y < noise_height; ++y)
    {
        for (int x = 0; x < noise_width; ++x)
        {
            const double noise = perlin1.normalizedOctave2D_01((x * 0.02), (y * 0.02), 5);

            array[x][y] = noise;
            // std::cout << "Noise: " << noise << std::endl;
            // std::cout << "Noise_values: " << noise_values[x][y] << std::endl;
            // std::cout << std::endl;
        }

        // std::cout << '\n';
    }



    for (int i = 0; i < noise_height; i++)
    {
        for (int j = 0; j < noise_width; j++)

        {
            noise_y = noise_values[i][j];


            // vertex
            noise_vertices.push_back( -noise_height/2.0f + noise_height*i/(float)noise_height );   // vx
            noise_vertices.push_back(  noise_y * 35 - noise_yShift);   // vy
            // std::cout << (y * 10 - yShift) << std::endl;
            noise_vertices.push_back( -noise_width/2.0f + noise_width*j/(float)noise_width );   // vz
        }
    }
    std::cout << "Loaded " << noise_vertices.size() / 3 << " vertices" << std::endl;

    vector<unsigned> indices;

    for (unsigned i = 0; i < noise_height - 1; i += noise_rez)
    {
        for (unsigned j = 0; j < noise_width; j += noise_rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + noise_width * (i + k * noise_rez));
            }
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;


    numStrips = (noise_height - 1) / noise_rez;
    numTrisPerStrip = (noise_width / noise_rez) * 2 - 2;

   // std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
   // std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;


    // first, configure the cube's VAO (and terrainVBO + terrainIBO)
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, noise_vertices.size() * sizeof(float), &noise_vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    vaoArray.push_back(terrainVAO);

}



void drawElements(int shader) {
    animatedTree(shader, ConeVAO, CylinderVAO, treetransform, barkTexture, pineTexture);    
}



void drawTerrain(int y) {

    glUseProgram(shaderHeightMap);
    glBindVertexArray(vaoArray.at(y));

    GLuint terrainTextureLocation = glGetUniformLocation(shaderHeightMap, "model");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned strip = 0; strip < numStrips; strip++)
    {
        glUseProgram(shaderHeightMap);
        glUniform1i(terrainTextureLocation, 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, barkTexture); 
        // setTerrainMatrix(shaderHeightMap, mat4(1.0f));
        // setWorldMatrix(shaderScene, terrainModelMatrix);  

        // mat4 worldMatrix = mat4(1.0f);
        // glUseProgram(shaderHeightMap);
        // glUniformMatrix4fv(terrainTextureLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        // setTerrainMatrix(shaderHeightMap, mat4(1.0f));

        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                        numTrisPerStrip+2,   // number of indices to render
                        GL_UNSIGNED_INT,     // index data type
                        (void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)); // offset to starting index
    }
}



void calculateTerrainGeneration() {

    int y = 0;

    for (auto i : terrainArray) {

        mat4 terrainMatrix =
            glm::translate(mat4(1.0f), i) *
            glm::rotate(mat4(1.0f), radians(180.0f), vec3(1.0f, 0.0f, 0.0f)) *
            glm::scale(mat4(1.0f), vec3(0.1f));


        setTerrainMatrix(shaderHeightMap, terrainMatrix);

        drawTerrain(vaoIndexArray.at(y));

        y++;


    } 

}

void deallocateResources() {
    glDeleteVertexArrays(1, &terrainVAO);
    glDeleteBuffers(1, &terrainVBO);
    glDeleteBuffers(1, &terrainIBO);
}



void Model::drawTexture(const GLuint& TextureID, int ShaderProgram, vec3 Scaling, vec3 Translate) {
    glDisable(GL_CULL_FACE);
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(glGetUniformLocation(ShaderProgram, "textureSampler"), 1);
    glBindBuffer(GL_ARRAY_BUFFER, this->VAO);
    mat4 Body = mat4(1.0f) * translate(mat4(1.0f), Translate) * scale(mat4(1.0f), Scaling);
    setWorldMatrix(ShaderProgram, Body);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    
}


void drawHouse(int ShaderProgram, GLuint textureIDpyramid, GLuint textureIDcube, GLuint textureIDbase,int ShapeObjpyramid, int ShapeObjcube)
{

    glBindVertexArray(ShapeObjpyramid);
    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(glGetUniformLocation(ShaderProgram, "textureSampler"), 1);
    mat4 Body = translate(mat4(1.0f), vec3(-12.0f, -2.5f, -12.0f)) * scale(mat4(1.0f), vec3(2.5f));
    mat4 HouseBody = Body * translate(mat4(1.0f), vec3(5.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 1.3f, 2.0f));
    glBindTexture(GL_TEXTURE_2D, textureIDpyramid);
    setWorldMatrix(ShaderProgram, HouseBody);
    glDrawArrays(GL_TRIANGLES, 0, numOfVerticesTexturedCone);
    glBindVertexArray(0);

    glBindVertexArray(ShapeObjcube);
    HouseBody = Body * translate(mat4(1.0f), vec3(5.0f, 1.2f, 0.0f)) * scale(mat4(1.0f), vec3(1.7f, 1.4f, 1.7f));
    setWorldMatrix(ShaderProgram, HouseBody);
    glBindTexture(GL_TEXTURE_2D, textureIDcube);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    HouseBody = Body * translate(mat4(1.0f), vec3(5.0f, 0.4f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 0.3f, 2.0f));
    setWorldMatrix(ShaderProgram, HouseBody);
    glBindTexture(GL_TEXTURE_2D, textureIDbase);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    


}

void drawPyramid(int ShaderProgram, GLuint textureIDpyramid, GLuint textureIDbase, int ShapeObjpyramid, int ShapeObjcube)
{

    glBindVertexArray(ShapeObjpyramid);
    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(glGetUniformLocation(ShaderProgram, "textureSampler"), 1);
    mat4 Body = translate(mat4(1.0f), vec3(12.0f, -2.0f, -9.0f));
    mat4 HouseBody = Body * translate(mat4(1.0f), vec3(19.0f, 2.5f, 2.0f)) * scale(mat4(1.0f), vec3(3.0f, 4.3f, 3.0f));;
    setWorldMatrix(ShaderProgram, HouseBody);
    glBindTexture(GL_TEXTURE_2D, textureIDpyramid);
    glDrawArrays(GL_TRIANGLES, 0, numOfVerticesTexturedCone);
    glBindVertexArray(0);

    glBindVertexArray(ShapeObjcube);

    HouseBody = Body * translate(mat4(1.0f), vec3(19.0f, 0.4f, 2.0f)) * scale(mat4(1.0f), vec3(3.5f, 0.3f, 3.5f));
    setWorldMatrix(ShaderProgram, HouseBody);
    glBindTexture(GL_TEXTURE_2D, textureIDbase);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);




}



void drawTower(int ShaderProgram, GLuint textureIDCone, GLuint textureIDCylinder, GLuint& CylinderVAO, GLuint& ConeVAO)
{

    glBindVertexArray(ConeVAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(glGetUniformLocation(ShaderProgram, "textureSampler"), 1);
    mat4 Body = translate(mat4(1.0f), vec3(-20.0f, -2.0f, 20.0f));
    mat4 HouseBody = Body * translate(mat4(1.0f), vec3(12.0f, 10.5f, 7.0f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f))* scale(mat4(1.0f), vec3(1.2f, 1.2f, 1.2f));;
    setWorldMatrix(ShaderProgram, HouseBody);
    glBindTexture(GL_TEXTURE_2D, textureIDCone);
    glDrawArrays(GL_TRIANGLES, 0, Coneindices.size());
    glBindVertexArray(0);

    glBindVertexArray(CylinderVAO);

    HouseBody = Body * translate(mat4(1.0f), vec3(12.0f, 0.4f, 7.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.05f, 1.003f, 1.05f));
    setWorldMatrix(ShaderProgram, HouseBody);
    glBindTexture(GL_TEXTURE_2D, textureIDCylinder);
    glDrawArrays(GL_TRIANGLES, 0, Cylinderindices.size());
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    



}


/* ---------- MAIN METHOD ---------- */
int main(int argc, char*argv[])
{
    //Create a glfwWindow instance
    CreateWindow glfwWindow = CreateWindow(windowWidth, windowHeight, windowTitle);
    GLFWwindow* currentWindow = glfwWindow.getWindow();

    blueTexture = loadTexture("assets/textures/blue-texture.jpg");
    redTexture = loadTexture("assets/textures/red-texture.jpg");
    pineTexture = loadTexture("assets/textures/pine.png");
    barkTexture = loadTexture("assets/textures/bark.png");
    StoneTexture = loadTexture("assets/textures/stoneTexture3.png");
    BrickTexture = loadTexture("assets/textures/Brick.png");
    RoofTexture =  loadTexture("assets/textures/woodboard.png");
    HouseWallTexture = loadTexture("assets/textures/HouseWall3.png");
    grassTexture = loadTexture("assets/textures/greenGrass.png");
    PyramidTexture = loadTexture("assets/textures/pyramidTexture.png");
    SandTexture = loadTexture("assets/textures/SandTexture.png");
    towerTopTexture = loadTexture("assets/textures/RoofTopTower.png");
    stoneWall = loadTexture("assets/textures/stoneWall.png");
    mushroomTexture = loadTexture("assets/textures/mushroomTexture.png");


    // Dimensions of the shadow texture, which should cover the viewport window size and shouldn't be oversized and waste resources
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

    // Variable storing index to texture used for shadow mapping
    GLuint depth_map_texture;
    // Get the texture
    glGenTextures(1, &depth_map_texture);
    // Bind the texture so the next glTex calls affect it
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
    // Create the texture and specify it's attributes, including widthn height, components (only depth is stored, no color information)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Set texture sampler parameters.
    // The two calls below tell the texture sampler inside the shader how to upsample and downsample the texture. Here we choose the nearest filtering option, which means we just use the value of the closest pixel to the chosen image coordinate.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range. Here we decide to just tile the image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Variable storing index to framebuffer used for shadow mapping
    GLuint depth_map_fbo; // fbo: framebuffer object
    // Get the framebuffer
    glGenFramebuffers(1, &depth_map_fbo);
    // Bind the framebuffer so the next glFramebuffer calls affect it
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    // Attach the depth map texture to the depth map framebuffer
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_map_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
    glDrawBuffer(GL_NONE); // disable rendering colors, only write depth values

    // Set the background color to gray
    glClearColor(0.176f, 0.164f, 0.164f, 1.0f);

    // Load the shaders from files
    shaderScene = loadSHADER(shaderPathPrefix + "scene_vertex.glsl", shaderPathPrefix + "scene_fragment.glsl");
    shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");
    shaderHeightMap = loadSHADER(shaderPathPrefix + "heightmap_vertex.vs", shaderPathPrefix + "heightmap_fragment.fs");

    // Terrain parameters
    vec3 terrainPosition(0.0f, -1.0f, 0.0f);

    // Camera parameters for view transform
    vec3 cameraPosition(0.0f, 8.0f, -8.0f);
    vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Other camera parameters
    float cameraSpeed = 3.0f;
    float cameraFastSpeed = 2.5 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable
    float viewField = 45.0f;
    float mousePos = 45.0f;
    float spinningAngle = 0.0f;
    


    // Set projection matrix for shader, this won't change
    projectionMatrix = glm::perspective(viewField,        // field of view in degrees
                                             800.0f / 600.0f,  // aspect ratio
                                             0.01f, 500.0f);   // near and far (near > 0)

    

    // Set initial view matrix
    viewMatrix = lookAt(cameraPosition,  // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp ); // up


    // Setprojection matrix on both shaders
    SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);

    // Set view matrix on both shaders
    SetUniformMat4(shaderScene, "view_matrix", viewMatrix);


    float lightAngleOuter = 30.0;
    float lightAngleInner = 20.0;
    // Set light cutoff angles on scene shader
    SetUniform1Value(shaderScene, "light_cutoff_inner", cos(radians(lightAngleInner)));
    SetUniform1Value(shaderScene, "light_cutoff_outer", cos(radians(lightAngleOuter)));

    // Set light color on scene shader
    SetUniformVec3(shaderScene, "light_color", vec3(1.0, 1.0, 1.0));

    // Set object color on scene shader
    SetUniformVec3(shaderScene, "object_color", vec3(1.0, 1.0, 1.0));

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(currentWindow, &lastMousePosX, &lastMousePosY);

    
    int cube_vao = createTexturedCubeVertexArrayObject();
    CylinderVAO = createCylinderVAO();
    ConeVAO = createConeVAO();
    pyramid_vao = createTexturedPyramidVertexArrayObject();
    AssetsService::initInstance();

    updateRand();

    //generateNoise(noise_values);

    stone = Model(AssetsService::getInstance()->getBoulderBig().getVAO(), AssetsService::getInstance()->getBoulderBig().getVectorSize(), vec3(1.0f));
    bench = Model(AssetsService::getInstance()->getBench().getVAO(), AssetsService::getInstance()->getBench().getVectorSize(), vec3(1.0f));
    grass1 = Model(AssetsService::getInstance()->getGrass1().getVAO(), AssetsService::getInstance()->getGrass1().getVectorSize(), vec3(1.0f));
    grass2 = Model(AssetsService::getInstance()->getGrass2().getVAO(), AssetsService::getInstance()->getGrass2().getVectorSize(), vec3(1.0f));
    grass3 = Model(AssetsService::getInstance()->getGrass3().getVAO(), AssetsService::getInstance()->getGrass3().getVectorSize(), vec3(1.0f));

    srand(time(0));


    for (int i = 0; i < 9; i++) {

        //unsigned int randomFloat1 = 10000 +  (rand()) / (RAND_MAX / (99999 - (10000)));
        seeds.push_back((unsigned int)(((std::rand() % 10000))));

        //seeds.push_back(randomFloat1);
        
        cout << seeds.at(i) << endl;
        siv::PerlinNoise perlin1{ seeds.at(i)};
        generateNoise(noise_values, perlin1);
        //cout << vaoArray.at(i) << endl;

    }

    terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z));
    //terrainArray.push_back(vec3(cameraPosition.x +10, -1.0f, cameraPosition.z));
    //terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 10));
    terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z));
    terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z));
    terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z + 25));
    terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z - 25));
    //terrainArray.push_back(vec3(cameraPosition.x - 5, -1.0f, cameraPosition.z));
    terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z + 25));
    terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z - 25));
    terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 25));
    terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z - 25));

    for (int i = 0; i < 9; i++) {
        int r = std::rand() % 9;
        vaoIndexArray.push_back(r);
        //cout << "r " << r << endl;
    }

    // Other OpenGL states to set once
    // Enable Backface culling
    //glEnable(GL_CULL_FACE);
    
    // Enable Depth Test   
    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(currentWindow)){
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;


        // Each frame, reset color of each pixel to glClearColor
        // Clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // light parameters
        lightPosition = vec3(10.0f, 10.0f, 30.0f); // the location of the light in 3D space
        // vec3(sinf(glfwGetTime() * 6.0f * 3.141592f), sinf(glfwGetTime() * 3.141592f), cosf(glfwGetTime() * 3.141592f));
        vec3 lightFocus(0.0f, 0.0f, 0.0f); // the point in 3D space the light "looks" at
        vec3 lightDirection = normalize(lightFocus - lightPosition);

        float lightNearPlane = 1.0f;
        float lightFarPlane = 180.0f;

        mat4 lightProjectionMatrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
        // perspective(20.0f, (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
        mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0.0f, 1.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set light space matrix on both shaders
        SetUniformMat4(shaderShadow, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(shaderScene, "light_view_proj_matrix", lightSpaceMatrix);

        // Set light far and near planes on scene shader
        SetUniform1Value(shaderScene, "light_near_plane", lightNearPlane);
        SetUniform1Value(shaderScene, "light_far_plane", lightFarPlane);

        // Set light position on scene shader
        SetUniformVec3(shaderScene, "light_position", lightPosition);

        // Set light direction on scene shader
        SetUniformVec3(shaderScene, "light_direction", lightDirection);

        // Spinning model rotation animation
        spinningAngle += 45.0f * dt; // This is equivalent to 45 degrees per second


        // Set model matrix and send to both shaders
        modelMatrix = //mat4(1.0f);
        glm::translate(mat4(1.0f), vec3(0.0f, 1.0f, -3.0f)) *
        glm::rotate(mat4(1.0f), radians(spinningAngle), vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(mat4(1.0f), vec3(0.1f));

        terrainModelMatrix = 
        glm::translate(mat4(1.0f), terrainPosition) *
        glm::rotate(mat4(1.0f), radians(180.0f), vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(mat4(1.0f), vec3(0.1f));


        SetUniformMat4(shaderScene, "model_matrix", modelMatrix);
        SetUniformMat4(shaderShadow, "model_matrix", modelMatrix);

        // Set the view matrix for first person camera and send to both shaders
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

        // Set view position on scene shader
        SetUniformVec3(shaderScene, "view_position", cameraPosition);

        // Set View and Projection matrices on both shaders
        setViewMatrix(shaderScene, viewMatrix);
        setProjectionMatrix(shaderScene, projectionMatrix);


        // Render shadow in 2 passes: 1- Render depth map, 2- Render scene
        // 1- Render shadow map:
        // a- use program for shadows
        // b- resize window coordinates to fix depth map output size
        // c- bind depth map framebuffer to output the depth values
        {
            // Use proper shader
            glUseProgram(shaderShadow);
            // Use proper image output size
            glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
            // Bind depth map texture as output framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
            //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Clear depth data on the framebuffer
            glClear(GL_DEPTH_BUFFER_BIT);

            setWorldMatrix(shaderShadow, mat4(1.0f));
            setProjectionMatrix(shaderShadow, mat4(1.0f));
            setViewMatrix(shaderShadow, mat4(1.0f));
            SetUniformMat4(shaderShadow, "model_matrix", mat4(1.0f));
            SetUniformMat4(shaderShadow, "light_view_proj_matrix", mat4(1.0f));

            drawElements(shaderShadow);

            glBindVertexArray(cube_vao);
            GLuint textureLocation = glGetUniformLocation(shaderShadow, "depth_map");
            glUniform1i(textureLocation, 0);
            glBindTexture(GL_TEXTURE_2D, blueTexture);
            mat4 cubeMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(80.0f, 40.0f, 80.0f));
            setWorldMatrix(shaderShadow, cubeMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

            // glBindTexture(GL_TEXTURE_2D, blueTexture);
            // mat4 lightCubeMatrix = translate(mat4(1.0f), lightPosition);
            // setWorldMatrix(shaderShadow, lightCubeMatrix);
            // glDrawArrays(GL_TRIANGLES, 0, 36); 

            drawHouse(shaderShadow,RoofTexture, HouseWallTexture, BrickTexture, pyramid_vao, cube_vao);
            drawPyramid(shaderShadow, PyramidTexture, SandTexture, pyramid_vao, cube_vao);
            drawTower(shaderShadow, towerTopTexture, stoneWall, CylinderVAO, ConeVAO);

        }

        // 2- Render scene: a- bind the default framebuffer and b- just render like what we do normally
        {
            // Use proper shader
            glUseProgram(shaderScene);
            // Use proper image output size
            // Side note: we get the size from the framebuffer instead of using WIDTH and HEIGHT because of a bug with highDPI displays
            int width, height;
            glfwGetFramebufferSize(currentWindow, &width, &height);
            glViewport(0, 0, width, height);
            // Bind screen as output framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Clear color and depth data on framebuffer
            glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depth_map_texture);

            GLuint shadowlocation = glGetUniformLocation(shaderScene, "shadow_map");
            glUniform1i(shadowlocation, 1);

            glActiveTexture(GL_TEXTURE0);

            drawElements(shaderScene);

            glBindVertexArray(cube_vao);
            glBindTexture(GL_TEXTURE_2D, blueTexture);
            mat4 cubeMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(80.0f, 40.0f, 80.0f));
            setWorldMatrix(shaderScene, cubeMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


            // glBindTexture(GL_TEXTURE_2D, blueTexture);
            // mat4 lightCubeMatrix = translate(mat4(1.0f), lightPosition);
            // setWorldMatrix(shaderScene, lightCubeMatrix);
            // glDrawArrays(GL_TRIANGLES, 0, 36); 
            
            drawHouse(shaderScene, RoofTexture, HouseWallTexture, BrickTexture, pyramid_vao, cube_vao);
            drawPyramid(shaderScene, PyramidTexture, SandTexture, pyramid_vao, cube_vao);
            drawTower(shaderScene, towerTopTexture, stoneWall, CylinderVAO, ConeVAO);

        }

    
        bool fastCam = glfwGetKey(currentWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(currentWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;
        
        double mousePosX, mousePosY;
        glfwGetCursorPos(currentWindow, &mousePosX, &mousePosY);
        
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;
        
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;
        
        const float cameraAngularSpeed = 50.0f;



        // glUseProgram(shaderScene);

        // GLuint textureLocation = glGetUniformLocation(shaderScene, "textureSampler");
        // glUniform1i(textureLocation, 0);
        // glBindVertexArray(cube_vao);
        // glBindTexture(GL_TEXTURE_2D, redTexture);
        // mat4 cubeMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
        // setWorldMatrix(shaderScene, cubeMatrix);
        // glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        SetUniformMat4(shaderHeightMap, "projection", projectionMatrix);
        SetUniformMat4(shaderHeightMap, "view", viewMatrix);
        SetUniformMat4(shaderHeightMap, "model", terrainModelMatrix);

        
        stone.drawTexture(StoneTexture, shaderScene, vec3(2, 1, 3), vec3(2,1,5));
        bench.drawTexture(redTexture, shaderScene,vec3(0.01f), vec3(1.0f, 2.0f,3.0f));
        grass1.drawTexture(grassTexture, shaderScene, vec3(0.1f,0.5f,0.3f), vec3(1.0,0.0f,1.0f));
        grass2.drawTexture(grassTexture, shaderScene, vec3(0.1f, 0.2f, 0.3f), vec3(2, 0, 2));
        grass3.drawTexture(mushroomTexture, shaderScene, vec3(0.1f, 0.2f, 0.3f), vec3(3, 0, 3));
        
        // drawHouse(shaderScene,RoofTexture, HouseWallTexture, BrickTexture, pyramid_vao, cube_vao);
        // drawPyramid(shaderScene, PyramidTexture, SandTexture, pyramid_vao, cube_vao);
        // drawTower(shaderScene, towerTopTexture, stoneWall, CylinderVAO, ConeVAO);

        /*renderTree(shaderScene, ConeVAO, CylinderVAO, barkTexture, leafTexture);*/
        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);
        
        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
        glm::normalize(cameraSideVector);


        //drawTerrain();
        calculateTerrainGeneration();
        drawElements(shaderShadow);



        if (cameraPosition.x <= -noise_height / 50 * terr_mult_3) {
            terrainArray.clear();

            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x +10, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 10));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z - 25));
            //terrainArray.push_back(vec3(cameraPosition.x - 5, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z - 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z - 25));
            cout << "Generating new chunk in [NEG X] axis" << endl;
            terr_mult_3 += 1.0f;
            terr_mult_1 -= 1.0f;

            vaoIndexArray.clear();

            for (int i = 0; i < 9; i++) {
                int r = std::rand() % 9;
                vaoIndexArray.push_back(r);
                //cout << "r " << r << endl;
            }

            updateRand();


        }

        if (cameraPosition.x >= noise_height / 50 * terr_mult_1) {
            terrainArray.clear();

            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x +10, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 10));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z - 25));
            //terrainArray.push_back(vec3(cameraPosition.x - 5, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z - 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z - 25));
            cout << "Generating new chunk in  axis" << endl;
            terr_mult_1 += 1.0f;
            terr_mult_3 -= 1.0f;

            vaoIndexArray.clear();

            for (int i = 0; i < 9; i++) {
                int r = std::rand() % 9;
                vaoIndexArray.push_back(r);
                //cout << "r " << r << endl;
            }

            updateRand();

        }

        if (cameraPosition.z >= noise_height / 50 * terr_mult_2) {
            terrainArray.clear();

            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x +10, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 10));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z - 25));
            //terrainArray.push_back(vec3(cameraPosition.x - 5, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z - 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z - 25));
            cout << "Generating new chunk in [NEG X] axis" << endl;
            terr_mult_2 += 1.0f;
            terr_mult_12 -= 1.0f;

            vaoIndexArray.clear();

            for (int i = 0; i < 9; i++) {
                int r = std::rand() % 9;
                vaoIndexArray.push_back(r);
                //cout << "r " << r << endl;
            }

            updateRand();

        }

        if (cameraPosition.z <= -noise_height / 50 * terr_mult_12) {

            //terrainArray.erase(terrainArray.begin());
            //terrainArray.erase(terrainArray.begin() + 1);
            //terrainArray.erase(terrainArray.begin() + 2);
            //terrainArray.erase(terrainArray.begin() + 3);
            //terrainArray.erase(terrainArray.begin() + 4);
            //terrainArray.erase(terrainArray.begin() + 5);
            //terrainArray.erase(terrainArray.begin() + 6);
            //terrainArray.erase(terrainArray.begin() + 7);
            //terrainArray.erase(terrainArray.begin() + 8);

            terrainArray.clear();

            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x +10, -1.0f, cameraPosition.z));
            //terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 10));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x + 25, -1.0f, cameraPosition.z - 25));
            //terrainArray.push_back(vec3(cameraPosition.x - 5, -1.0f, cameraPosition.z));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x - 25, -1.0f, cameraPosition.z - 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z + 25));
            terrainArray.push_back(vec3(cameraPosition.x, -1.0f, cameraPosition.z - 25));

            // terrainSize.z - cameraPosition.z;
            cout << "Generating new chunk in [NEG Z] axis" << endl;
            terr_mult_12 += 1.0f;
            terr_mult_2 -= 1.0f;

            vaoIndexArray.clear();

            for (int i = 0; i < 9; i++) {
                int r = std::rand() % 9;
                vaoIndexArray.push_back(r);
                cout << "r " << r << endl;
            }

            updateRand();

        }




        // ----- Handle Keyboard and Mouse inputs -----  

        // End Frame
        glfwSwapBuffers(currentWindow);

        // Wait/look for inputs
        glfwPollEvents();


        if(cameraPosition.y < -1.5f) {

           cameraPosition.y += 1.0f;


       }

       if(cameraPosition.x > 39.0f) {

           cameraPosition.x += -1.0f;


       }

       else if(cameraPosition.z > 39.0f)   {

           cameraPosition.z += -1.0f;


       }

       if(cameraPosition.y >=18.0f) {

           cameraPosition.y -= 1.0f;


       }

       else if(cameraPosition.x <= -39.0f) {

           cameraPosition.x += 1.0f;


       }

       else if(cameraPosition.z <= -39.0f)   {

           cameraPosition.z += 1.0f;


       }



       //if (glfwGetKey(currentWindow, GLFW_KEY_V) == GLFW_PRESS) // move camera to the left
       //{
       //               cout << "x: " << cameraPosition.x << endl;
       //               cout << "y: " << cameraPosition.y << endl;
       //               cout << "z: " << cameraPosition.z << endl;

       //    //cout << "H" << cameraHorizontalAngle << endl;
       //    //cout << "V" << cameraVerticalAngle << endl;
       //}


        if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(currentWindow, true);
        }  
        if (glfwGetKey(currentWindow, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(currentWindow, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(currentWindow, GLFW_KEY_W) == GLFW_PRESS) // move camera up
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(currentWindow, GLFW_KEY_S) == GLFW_PRESS) // move camera down
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(currentWindow, GLFW_KEY_U) == GLFW_PRESS) // move camera down
        {
            cout << "Camera Position: x: " << cameraPosition.x << " y: " << cameraPosition.y  << " z: " << cameraPosition.z  << endl;
            cout << "Terrain Position: x: " << terrainPosition.x << " y: " << terrainPosition.y << " z: " << terrainPosition.z << endl;
        }

        // If the mouse isn't pressed, set the camera angles
        if (!glfwGetMouseButton(currentWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
        {
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
            cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
        }
    }

    deallocateResources();
    glfwWindow.close();
}