#pragma once

#include "Cylinder.h"
#include <cstdlib>
#include <time.h>

float treeConeTransformDistance = 0.1;
float treeConeTransformDelta = 0.025;
mat4 treetransform;
mat4 mainMatrix;
vector<vec3> coordinates;

void renderTree(const GLuint &textureShader, GLuint ConVAO, GLuint CylVAO, mat4 treetransform, GLuint bark, GLuint leaf)
{

    GLuint assettextureLocation = glGetUniformLocation(textureShader, "textureSampler");
    GLuint assetMatrixLocation = glGetUniformLocation(textureShader, "model_matrix");

    glm::mat4 treeConeTransform = glm::rotate(glm::mat4(1.0f), radians(2.5f), glm::vec3(0.0f, treeConeTransformDistance, 0.0f));

    mainMatrix = translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.35f));

    if (treeConeTransformDistance > 1.5 || treeConeTransformDistance < -1.5)
    {
        treeConeTransformDelta = -treeConeTransformDelta;
    }

    treeConeTransformDistance += treeConeTransformDelta;

    srand(time(0));

    for (int i = 0; i < 50; i++)
    {

        //float randomFloat1 = -50 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50-(-50))));
        //float randomFloat2 = -50 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50-(-50))));

        //coordinates.push_back(vec3(randomFloat1, 0.0f, randomFloat2));

        mainMatrix = mainMatrix * translate(mat4(1.0f), coordinates.at(i));

        glBindVertexArray(CylVAO);
        glUniform1i(assettextureLocation, 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bark);
        mat4 cylinderMatrix = mainMatrix * translate(mat4(1.0f), vec3(0.0f, -2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.15f, 0.3f, 0.15f)) * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        glUseProgram(textureShader);
        glUniformMatrix4fv(assetMatrixLocation, 1, GL_FALSE, &cylinderMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, Coneindices.size());

        mat4 coneMatrix;

        for (int i = 0; i < 3; i++)
        {
            glBindVertexArray(ConeVAO);
            glUniform1i(assettextureLocation, 0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, leaf);

            switch (i)
            {
            case 0:
            case 2:
                coneMatrix = mainMatrix * translate(mat4(1.0f), vec3(0.0f, 0.5f + i * 1.15, 0.0f)) * scale(mat4(1.0f), vec3(0.5f - i * 0.1f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) * treeConeTransform;
                break;
            case 1:
                coneMatrix = mainMatrix * translate(mat4(1.0f), vec3(0.0f, 0.5f + i * 1.15, 0.0f)) * scale(mat4(1.0f), vec3(0.5f - i * 0.1f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) * inverse(treeConeTransform);
                break;
            default:
                break;
            }

            glUseProgram(textureShader);
            glUniformMatrix4fv(assetMatrixLocation, 1, GL_FALSE, &coneMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, Coneindices.size());
        }
    }

    // glEnableVertexAttribArray(0);
    // glBindVertexArray(CylVAO);
    // glUseProgram(textureShader);

    // glActiveTexture(GL_TEXTURE0);

    // mat4 cylinderMatrix;

    // //GLuint MVP_Cylinder = glGetUniformLocation(textureShader, "model_matrix");

    // mat4 Cyl;

    // glFrontFace(GL_CW);
    // glUniform1ui(glGetUniformLocation(textureShader, "textureSampler"), 1);

    // Cyl = cylinderMatrix * translate(mat4(1.0f), vec3(10.0f, 0.0f, -5.0f)) * scale(mat4(1.0f), vec3(0.5f)) * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    // glUniformMatrix4fv(textureShader, 1, GL_FALSE, &Cyl[0][0]);
    // glBindTexture(GL_TEXTURE_2D, bark);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, Coneindices.size());

    // glBindVertexArray(ConVAO);

    // glFrontFace(GL_CW);

    // Cyl = cylinderMatrix * translate(mat4(1.0f), vec3(20.0f, 0.0f, 15.0f)) * scale(mat4(1.0f), vec3(1.5f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) * treetransform;
    // glUniformMatrix4fv(textureShader, 1, GL_FALSE, &Cyl[0][0]);
    // glBindTexture(GL_TEXTURE_2D, leaf);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, Coneindices.size());

    // Cyl = cylinderMatrix * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) * translate(mat4(1.0f), vec3(10.0f, 0.0f, 20.0f)) * inverse(treetransform) * scale(mat4(1.0f), vec3(1.25f));
    // glUniformMatrix4fv(textureShader, 1, GL_FALSE, &Cyl[0][0]);
    // glBindTexture(GL_TEXTURE_2D, leaf);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, Coneindices.size());

    // Cyl = cylinderMatrix * translate(mat4(1.0f), vec3(10.0f, 0.0f, 25.0f)) * treetransform * scale(mat4(1.0f), vec3(1.0f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    // glUniformMatrix4fv(textureShader, 1, GL_FALSE, &Cyl[0][0]);
    // glBindTexture(GL_TEXTURE_2D, leaf);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, Coneindices.size());

    // glBindVertexArray(0);
}

void animatedTree(int textureShader, GLuint ConVAO, GLuint CylVAO, mat4 treetransform, GLuint bark, GLuint leaf)
{
    renderTree(textureShader, ConVAO, CylVAO, treetransform, bark, leaf);
}

void updateRand() {

    coordinates.clear();

    for (int i = 0; i < 100; i++) {

        float randomFloatX = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 - (-50))));
        float randomFloatZ = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 - (-50))));
    
        coordinates.push_back(vec3(randomFloatX, 0.0f, randomFloatZ));

    }

}