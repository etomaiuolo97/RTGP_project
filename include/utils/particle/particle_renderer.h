#pragma once

#ifndef PARTICLE_RENDERER
#define PARTICLE_RENDERER

#include "utils/system/renderer.h"
#include "utils/particle/particle_generator.h"

class ParticleRenderer : public Renderer{
private:
    ParticleGenerator generator;
    ParticleProps particle;

    

public:

    ParticleRenderer (glm::mat4 projection)
        :Renderer(projection){            
            Renderer::SetupShaders(generator.getShader().getProgram());

            this->generator.getShader().start();
            this->generator.getShader().loadProjection(this->projection);
            this->generator.getShader().stop();

            this->particle.colorBegin = {0.8f, 0.8f, 1.0f, 1.0f};
            this->particle.colorEnd = {1.0f, 1.0f, 1.0f, 1.0f};

            this->particle.sizeBegin = 0.01f;
            this->particle.sizeVariation = 0.01f;
            this->particle.sizeEnd = 0.1f;

            this->particle.life = 1.1f;

            this->particle.velocity = {0.0f, 0.0f, 0.0f};
            this->particle.velocityVariation = 4.0f;

            this->particle.position = {0.0f, 3.1f, -5.0f};
    }

    void render(GLfloat deltaTime, Camera & camera, glm::vec3 lightPosition, GLint tCube) {
        glm::mat4 view = Renderer::createViewMatrix(camera);

        this->generator.getShader().start();
        this->generator.getShader().loadView(view);
        camera.setPitch(-camera.getPitch());
        this->generator.getShader().loadCameraPosition(camera.getPosition());
        camera.setPitch(-camera.getPitch());

        this->generator.getShader().loadPointLight(lightPosition);

        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        glCall(glDepthMask(false));

        for (GLuint i = 0; i < 10; i++){
            this->generator.emit(particle);
        }

        glCall(glActiveTexture(GL_TEXTURE0));
        glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, tCube));

        this->generator.getShader().loadTCube(0);
        this->generator.update(deltaTime);
        this->generator.Draw(view);

        glCall(glDepthMask(true));
        glCall(glDisable(GL_BLEND));

        generator.getShader().stop();        
    }

    void cleanUp(){
        generator.cleanUp();
    }
};

#endif