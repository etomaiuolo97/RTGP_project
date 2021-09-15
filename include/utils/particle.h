#pragma once

#include <glfw/glfw3.h>
#include <utils/particleTexture.h>

#define GRAVITY -50

class Particle {

    private:

        glm::vec3 position, velocity;
        float gravityEffect, lifeLength;
        float rotation, scale;

        particleTexture texture;
        glm::vec2 texOffset1;
        glm::vec2 texOffset2;
        float blend;

        float elapsedTime;
        float distance;

        // TODO: texture with model
        void updateTextureCoordInfo(){
            float lifeFactor = elapsedTime/lifeLength;
            int stageCount = texture.getNumberRows() * texture.getNumberRows();
            float atlasProgression = lifeFactor * stageCount;
            int index1 = floor(atlasProgression);
            int index2 = index1 < stageCount -1 ? index1 + 1 : index1;
            this->blend = (int)atlasProgression % 1;
            setTextureOffset(texOffset1,index1);
            setTextureOffset(texOffset2,index2);
            
        }

        void setTextureOffset(glm::vec2 offset, int index){
            int column = index % texture.getNumberRows();
            int row = index / texture.getNumberRows();
            offset.x = (float)column / texture.getNumberRows();
            offset.y = (float)row / texture.getNumberRows();
        }

    public:
        // TODO: array di particelle
        Particle(particleTexture texture,glm::vec3 x, glm::vec3 v,float gravity, float life, float rot,
            float scale){
            this->texture=texture;
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

        glm::vec2 getTexOffset1(){
            return texOffset1;
        }

        glm::vec2 getTexOffset2(){
            return texOffset2;
        }

        float getBlend(){
            return blend;
        }

        particleTexture getTexture(){
            return this->texture;
        }

        float getDistance(){
            return this->distance;
        }

        // dice se la particella è ancora viva
        bool update(Camera camera){
            velocity.y += GRAVITY * gravityEffect * glfwGetTime();
            glm::vec3 change = glm::vec3(velocity);
            change*=(float)glfwGetTime();
            //distance = camera.position - position;
            distance = sqrt((camera.position.x - position.x)*(camera.position.x - position.x) + (camera.position.y - position.y)*(camera.position.y - position.y)
                             + (camera.position.z - position.z)*(camera.position.z - position.z));
            position+=change;
            elapsedTime += (float)glfwGetTime();

            return elapsedTime < lifeLength;
        }

        Particle();

};