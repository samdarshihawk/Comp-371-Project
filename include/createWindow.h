#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 

struct CreateWindow
{
    public:
        //Constructor
        CreateWindow(int width, int height, char *windowName);

        //Member functions
        int initializeWindow(int width, int height, char *windowName);
        int close();

        //Getter and Setter
        GLFWwindow* getWindow() { return window; }
        void setWindow(GLFWwindow* _window) { window = _window; }

    private:
        GLFWwindow* window;
};