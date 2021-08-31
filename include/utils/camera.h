#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592653

enum Camera_Movement {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD
};

const GLfloat SPEED = 4.0f;

class Camera {

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 7.0f);
    glm::vec3 FountainRot = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat pitch = 30;
    GLfloat yaw = 0;
    GLfloat roll;

    Camera () {};

    Camera (glm::vec3);

    ~Camera();

    void move (bool keys [], GLfloat deltaTime){
        if (keys[GLFW_KEY_LEFT])
            this->position.x -= SPEED;
        if (keys[GLFW_KEY_RIGHT])
            this->position.x += SPEED;
        if (keys[GLFW_KEY_UP])
            this->position.z -= SPEED;
        if (keys[GLFW_KEY_DOWN])
            this->position.z += SPEED;
    }

    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
        GLfloat change = deltaTime * SPEED;

        switch(direction){
            case UP:
                this->pitch += change;
                break;
            
            case DOWN:
                this->pitch -= change;
                break;
            
            case RIGHT:
                this->angleAroundFountain += change;
                break;
            
            case LEFT:
                this->angleAroundFountain -= change;
                break;
            
            case BACKWARD:
                this->disFromFountain += change;
                break;
            
            case FORWARD:
                this->disFromFountain -= change;
                break;
        }
    }
    
    glm::mat4 GetViewMatrix() {
        GLfloat hDis = this->calculateHorizontalDistance();
        GLfloat vDis = this->calculateVerticalDistance();
        
        this->calculateCameraPos(hDis, vDis);
        this->yaw = 180 - (this->FountainRot.y + this->angleAroundFountain);

        return glm::lookAt(this->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

private:
    GLfloat disFromFountain = 20;
    GLfloat angleAroundFountain = 0;

    GLfloat calculateHorizontalDistance() {
        return (GLfloat) (this->disFromFountain * cos(glm::radians(pitch)));
    }

    GLfloat calculateVerticalDistance() {
        return (GLfloat) (this->disFromFountain * sin(glm::radians(pitch)));
    }

    void calculateCameraPos (GLfloat hDis, GLfloat vDis) {
    
        GLfloat offsetX = (GLfloat)(hDis * sin(glm::radians(this->angleAroundFountain)));
        GLfloat offsetZ = (GLfloat)(hDis * cos(glm::radians(this->angleAroundFountain)));
        
    
        this->position.x = offsetX;
        this->position.z = offsetZ;
        this->position.y = vDis;
    }
};

Camera::Camera(glm::vec3 position) {
    this->position = position;
}

Camera::~Camera() {
}

