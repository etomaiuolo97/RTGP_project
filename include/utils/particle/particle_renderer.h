#pragma once

#ifndef PARTICLE_RENDERER
#define PARTICLE_RENDERER

#include "utils/system/renderer.h"
#include "utils/particle/particle_generator.h"

class ParticleRenderer : public Renderer{
private:
    ParticleGenerator generator;
    ParticleProps particle;

    bool semaforo = true;

public:

    ParticleRenderer (glm::mat4 projection)
        :Renderer(projection){            
            Renderer::SetupShaders(generator.getShader().getProgram());

            this->generator.getShader().start();
            this->generator.getShader().loadProjection(this->projection);
            this->generator.getShader().stop();

            this->particle.colorBegin = {254/255.0f, 212/255.0f, 123/255.0f, 1.0f};
            this->particle.colorEnd = {254/255.0f, 109/255.0f, 41/255.0f, 1.0f};
            this->particle.sizeBegin = 0.8f;
            this->particle.sizeVariation = 0.2f;
            this->particle.sizeEnd = 0.0f;
            this->particle.life = 1.0f;
            this->particle.velocity = {0.0f, 0.0f, 0.0f};
            this->particle.velocityVariation = 2.0f;
            this->particle.position = {0.0f, 0.0f, -3.0f};
    }

    void render(GLfloat deltaTime, Camera & camera) {
        this->generator.getShader().start();
        
        this->generator.getShader().loadView(camera.GetViewMatrix());

        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        for (GLuint i = 0; i < 5; i++){
            generator.emit(particle);
            semaforo = false;
        }

        generator.update(deltaTime);
        generator.Draw();

        glCall(glDisable(GL_BLEND));

        generator.getShader().stop();        
    }

    void cleanUp(){
        generator.cleanUp();
    }
};

#endif