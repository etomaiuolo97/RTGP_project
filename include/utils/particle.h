#pragma once

#include <glfw/glfw3.h>

#define GRAVITY -50

class Particle {

    private:

        glm::vec3 position, velocity;
        float gravityEffect, lifeLength;
        float rotation, scale;

        float elapsedTime;

    public:

        Particle(glm::vec3 x, glm::vec3 v,float gravity, float life, float rot,
            float scale){
            this->position=x;
            this->velocity=v;
            this->gravityEffect=gravity;
            this->lifeLength=life;
            this->rotation=rot;
            this->scale=scale;
            particleMaster pMaster;
            // Add particles to the particleMaster class
            pMaster.addParticle(*this);
        }

        glm::vec3 getPosition(){
            return this->position;
        }

        float getRotation(){
            return this->rotation;
        }

        float getScale(){
            return this->scale;
        }

        bool update(){
            velocity.y += GRAVITY * gravityEffect * glfwGetTime();
            glm::vec3 change = glm::vec3(velocity);
            change*=(float)glfwGetTime();
            position+=change;
            elapsedTime += (float)glfwGetTime();

            return elapsedTime < lifeLength;
        }

};