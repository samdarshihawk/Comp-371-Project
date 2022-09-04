#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "OBJloader.h"

//#if defined(PLATFORM_OSX)
//const std::string basePath = "Models";
//#else
//const std::string basePath = "../Assets/Models";
//#endif

class Asset {
public:
	Asset() = default;
	Asset(const std::string& path);

	GLuint getVAO() { return this->VAO; };
	GLuint getVectorSize() { return this->numVertices; };

private:
	GLuint VAO;
	GLuint numVertices;
};

class AssetsService {
public:
	static AssetsService* getInstance();
	static void initInstance();
	static void resetInstance();

	Asset getBoulderBig() { return this->boulderBig; };
	Asset getBoulderMed() { return this->boulderMed; };
	Asset getBoulderSmall() { return this->boulderSmall; };

	Asset getBench() { return this->bench; };
	
	Asset getGrass1() { return this->grass1; };
	Asset getGrass2() { return this->grass2; };
	Asset getGrass3() { return this->grass3; };

	Asset getBallTree() { return this->ballTree; };
	Asset getEdgyPineTree() { return this->edgyPineTree; };
	Asset getStumpyPineTree() { return this->stumpyPineTree; };
	Asset getSeussBallTree() { return this->seussBallTree; };


private:
	AssetsService();
	static AssetsService* instance;
	Asset boulderBig;
	Asset boulderMed;
	Asset boulderSmall;

	Asset bench;
	Asset grass1;
	Asset grass2;
	Asset grass3;
	Asset grass4;

	Asset ballTree;
	Asset edgyPineTree;
	Asset stumpyPineTree;
	Asset seussBallTree;

	
};

const std::string boulderBigAsset = "assets/Models/boulder_big.obj"; 
const std::string benchAsset = "assets/Models/bench.obj";
const std::string grassType1Asset = "assets/Models/GrassType1.obj"; 
const std::string grassType2Asset = "assets/Models/GrassType2.obj";
const std::string grassType3Asset = "assets/Models/GrassType3.obj";