/**
 * Adaptation of https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/camera.h
 */
#pragma once

#ifndef CAMERA
#define CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592653
// It defines several possible options for camera movement.
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
    Camera () {
        this->calculateCameraPos(this->calculateHorizontalDistance(), this->calculateVerticalDistance());
    };

    ~Camera() {};
    /**
     * @brief processes input received from any keyboard-like input system. Accepts input parameter in the
     *  form of camera defined ENUM (to abstract it from windowing systems)
     * 
     * @param direction options for camera movement
     * @param deltaTime time
     */
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

        GLfloat hDis = this->calculateHorizontalDistance();
        GLfloat vDis = this->calculateVerticalDistance();
        
        this->calculateCameraPos(hDis, vDis);
        this->yaw = 180 - (this->angleAroundFountain);
    }
    /**
     * @brief processes input received from a mouse scroll-wheel event. It only requires input on the vertical wheel-axis
     * @param offset 
     * 
     */
    void calculateZoom (GLfloat offset){
        if (this->disFromFountain >= 1.0f && this->disFromFountain <= 45.0f)
            this->disFromFountain -= offset;
        if (this->disFromFountain <= 1.0f)
            this->disFromFountain = 1.0f;
        if (this->disFromFountain >= 45.0f)
            this->disFromFountain = 45.0f;

        this->calculateCameraPos(this->calculateHorizontalDistance(), this->calculateVerticalDistance());
    }

    glm::vec3 getPosition () {
        return this->position;
    }

    void setPosition (glm::vec3& position) {
        this->position = glm::vec3(position.x, position.y, position.z);
    }

    GLfloat getPitch (){
        return this->pitch;
    }

    void setPitch (GLfloat pitch) {
        this->pitch = pitch;
    }
    
    GLfloat getYaw () {
        return this->yaw;
    }

    void setYaw (GLfloat yaw) {
        this->yaw = glm::mod(yaw, 360.0f);
    }

    GLfloat getRoll () {
        return this->roll;
    }

    void setRoll (GLfloat roll) {
        this->roll = roll;
    }

    glm::vec3 getObjPosition () {
        return this->objPosition;
    }

    void setObjPosition (glm::vec3 objPosition) {
        this->objPosition = objPosition;
    }

private:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -7.0f);
    glm::vec3 objPosition = glm::vec3(0.0f, -3.0f, -5.0f);

    GLfloat pitch = 30;
    GLfloat yaw = 180;
    GLfloat roll = 0;

    GLfloat disFromFountain = 30;
    GLfloat angleAroundFountain = 0;
    
    GLfloat calculateHorizontalDistance() {
        return (GLfloat) (this->disFromFountain * cos(glm::radians(pitch)));
    }
    
    GLfloat calculateVerticalDistance() {
        return (GLfloat) (this->disFromFountain * sin(glm::radians(pitch)));
    }

    /**
     * @brief Calculate position of the camera through trigonometry
     * 
     * @param hDis is the horizontal distance
     * @param vDis is the vertical distance
     */
    void calculateCameraPos (GLfloat hDis, GLfloat vDis) {
        GLfloat offsetX = (GLfloat)(hDis * sin(glm::radians(this->angleAroundFountain)));
        GLfloat offsetZ = (GLfloat)(hDis * cos(glm::radians(this->angleAroundFountain)));
        
        this->position.x = this->objPosition.x - offsetX;
        this->position.z = this->objPosition.z - offsetZ;
        this->position.y = this->objPosition.y + vDis;
    }
};


#endif