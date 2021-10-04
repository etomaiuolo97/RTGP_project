#pragma once

#ifndef PARTICLE_RENDERER
#define PARTICLE_RENDERER

#include "utils/system/renderer.h"
#include "utils/particle/particle_generator.h"
#include "utils/particle/particle_framebuffers.h"

class ParticleRenderer : public Renderer{
private:
    const GLfloat WAVE_SPEED = 0.1f;

    ParticleGenerator generator;
    ParticleProps particle;
    ParticleFrameBuffers fbos;

    GLfloat moveFactor = 0;

    GLuint dudvTexture, normalMapTexture;

    void prepareRender (Camera & camera, GLfloat deltaTime, Light & light) {
        this->generator.getShader().start();
        
        this->generator.getShader().loadViewMatrix(Renderer::createViewMatrix(camera));
        this->generator.getShader().loadCameraPosition(camera.getPosition());

        this->moveFactor += WAVE_SPEED * deltaTime;
        this->moveFactor = glm::mod(this->moveFactor, 1.0f);
        this->generator.getShader().loadMoveFactor(this->moveFactor);
        this->generator.getShader().loadLight(light.position, light.color);

        glCall(glActiveTexture(GL_TEXTURE0));
        glCall(glBindTexture(GL_TEXTURE_2D, this->fbos.getReflectionTexture()));
        glCall(glActiveTexture(GL_TEXTURE1));
        glCall(glBindTexture(GL_TEXTURE_2D, this->fbos.getRefractionTexture()));
        glCall(glActiveTexture(GL_TEXTURE2));
        glCall(glBindTexture(GL_TEXTURE_2D, this->dudvTexture));
        glCall(glActiveTexture(GL_TEXTURE3));
        glCall(glBindTexture(GL_TEXTURE_2D, this->normalMapTexture));
        glCall(glActiveTexture(GL_TEXTURE4));
        glCall(glBindTexture(GL_TEXTURE_2D, this->fbos.getRefractionDepthTexture()));
        
        this->generator.getShader().connectTextureUnits();

        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

public:

    ParticleRenderer (glm::mat4 projection)
        :Renderer(projection){            
            Renderer::SetupShaders(generator.getShader().getProgram());

            this->generator.getShader().start();
            this->generator.getShader().loadProjectionMatrix(this->projection);
            this->generator.getShader().stop();

            this->particle.colorBegin = {0.8f, 0.8f, 1.0f, 1.0f};
            this->particle.colorEnd = {1.0f, 1.0f, 1.0f, 1.0f};

            this->particle.sizeBegin = 0.01f;
            this->particle.sizeVariation = 0.01f;
            this->particle.sizeEnd = 0.1f;

            this->particle.life = 1.5f;

            this->particle.velocity = {0.0f, 0.0f, 0.0f};
            this->particle.velocityVariation = 4.0f;

            this->particle.position = {0.0f, 4.5f, -5.0f};

            this->fbos = ParticleFrameBuffers(WIDTH, HEIGHT);

            this->dudvTexture = LoadTexture("./textures/water/DuDv.png", true);
            this->normalMapTexture = LoadTexture("./textures/water/matchingNormalMap.png", true);

    }

    void render(GLfloat deltaTime, Camera & camera, Light light, GLint tCube) {
        prepareRender(camera, deltaTime, light);

        for (GLuint i = 0; i < 10; i++){
            this->generator.emit(particle);
        }
        
        this->generator.update(deltaTime);
        this->generator.Draw(Renderer::createViewMatrix(camera));

        glCall(glDepthMask(true));
        glCall(glDisable(GL_BLEND));

        generator.getShader().stop();        
    }

    void bindReflectionFrameBuffer () {
        this->fbos.bindReflectionFrameBuffer();
    }

    void unbindCurrentFrameBuffer () {
        this->fbos.unbindCurrentFrameBuffer();
    }

    void bindRefractionFrameBuffer (){
        this->fbos.bindRefractionFrameBuffer();
    }

    void cleanUp(){
        this->fbos.cleanUp();
        this->generator.cleanUp();
    }
};

#endif