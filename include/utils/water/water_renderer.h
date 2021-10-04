#pragma once

#ifndef WATER_RENDERER
#define WATER_RENDERER

#include "utils/system/renderer.h"
#include "utils/system/model.h"

#include "utils/water/water_shader.h"
#include "utils/water/water_tile.h"
#include "utils/water/water_framebuffers.h"

class WaterRenderer : public Renderer{
private:
    const GLfloat WAVE_SPEED = 0.1f;

    WaterShader shader;
    WaterFrameBuffers fbos;
    WaterTile tile = WaterTile (0.0f, -5.0f, 0.1f);
    Model quad;

    GLfloat moveFactor = 0;

    GLuint dudvTexture, normalMapTexture;

    void prepareRender (Camera & camera, GLfloat deltaTime, glm::vec3 lightPosition, glm::vec3 lightColour) {
        shader.start();
        
        shader.loadViewMatrix(Renderer::createViewMatrix(camera));
        shader.loadCameraPosition(camera.getPosition());

        this->moveFactor += WAVE_SPEED * deltaTime;
        this->moveFactor = glm::mod(this->moveFactor, 1.0f);
        shader.loadMoveFactor(this->moveFactor);
        shader.loadLight(lightPosition, lightColour);

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
        
        shader.connectTextureUnits();

        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    void unbind(){
        glCall(glDisable(GL_BLEND));
        shader.stop();
    }

public:

    WaterRenderer (glm::mat4 projection, GLuint width, GLuint height)
        :Renderer(projection){ 
        
        Renderer::SetupShaders(shader.getProgram());
          
        this->fbos = WaterFrameBuffers(width, height);
        this->dudvTexture = LoadTexture("./textures/water/DuDv.png", true);
        this->normalMapTexture = LoadTexture("./textures/water/matchingNormalMap.png", true);

        shader.start();
        shader.loadProjectionMatrix(projection);
        shader.stop();
        quad = Model("./meshes/circle.obj");
    }

    void render(Camera & camera, GLfloat deltaTime, glm::vec3 lightPosition, glm::vec3 lightColour) {
        prepareRender(camera, deltaTime, lightPosition, lightColour);

        glm::mat4 modelMatrix = Renderer::createTransformationMatrix(
            glm::vec3(this->tile.getX(), this->tile.getHeight(), this->tile.getZ()),
            0, 0, 0, this->tile.TILE_SIZE
        );
        shader.loadModelMatrix(modelMatrix);

        quad.Draw();

        unbind();
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
        shader.cleanUp();
        fbos.cleanUp();
    }

    GLfloat getWaterHeight() {
        return this->tile.getHeight();
    }

};

#endif