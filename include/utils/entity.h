#pragma once

using namespace std;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/textured_model.h>

class Entity {
public:
    TexturedModel model;
    glm::vec3 position;
    GLfloat rotX, rotY, rotZ;
    GLfloat scale;

    Entity (TexturedModel model, glm::vec3 position, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat scale){
        this->model = model;
        this->position = position;
        this->rotX = rotX;
        this->rotY = rotY;
        this->rotZ = rotZ;
        this->scale = scale;
    }

    void increasePosition (GLfloat dx, GLfloat dy, GLfloat dz) {
        this->position.x += dx;
        this->position.y += dy;
        this->position.z += dz;
    }

    void increaseRotation (GLfloat dx, GLfloat dy, GLfloat dz) {
        this->rotX += dx;
        this->rotY += dy;
        this->rotZ += dz;
    }

};