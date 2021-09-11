#pragma once

#ifndef WATER_RENDERER
#define WATER_RENDERER

#include "utils/renderer/renderer.h"
#include "utils/water_shader.h"
#include "utils/water_tile.h"
#include "utils/model.h"
#include "utils/waterframe_buffers.h"

class WaterRenderer : public Renderer{
private:
    const GLfloat WAVE_SPEED = 0.03f;

    WaterShader shader;
    Model quad;
    WaterFrameBuffers fbos;

    GLfloat moveFactor = 0;

    GLuint dudvTexture, normalMapTexture;

    void prepareRender (Camera camera, GLfloat deltaTime, glm::vec3 lightPosition, glm::vec3 lightColour) {
        shader.start();
        shader.loadViewMatrix(camera.GetViewMatrix());
        shader.loadCameraPosition(camera.position);

        this->moveFactor += WAVE_SPEED * deltaTime;
        this->moveFactor = glm::mod(this->moveFactor, 1.0f);
        shader.loadMoveFactor(this->moveFactor);
        shader.loadLight(lightPosition, lightColour);

        glCall(glActiveTexture(GL_TEXTURE0));
        glCall(glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture()));
        glCall(glActiveTexture(GL_TEXTURE1));
        glCall(glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture()));
        glCall(glActiveTexture(GL_TEXTURE2));
        glCall(glBindTexture(GL_TEXTURE_2D, this->dudvTexture));
        glCall(glActiveTexture(GL_TEXTURE3));
        glCall(glBindTexture(GL_TEXTURE_2D, this->normalMapTexture));
        glCall(glActiveTexture(GL_TEXTURE3));
        glCall(glBindTexture(GL_TEXTURE_2D, this->fbos.getRefractionDepthTexture()));

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
        this->fbos = WaterFrameBuffers(width, height);
        this->dudvTexture = LoadTexture("../textures/water/DuDv.png");
        this->normalMapTexture = LoadTexture("../textures/water/normalMap.png");

        shader.start();
        shader.connectTextureUnits();
        shader.loadProjectionMatrix(projection);
        shader.stop();
        quad = Model("../meshes/circle.obj");
    }

    WaterFrameBuffers getFbos(){
        return this->fbos;
    }

    void render(vector<WaterTile> water, Camera camera, GLfloat deltaTime, glm::vec3 lightPosition, glm::vec3 lightColour) {
        prepareRender(camera, deltaTime, lightPosition, lightColour);

        for (WaterTile tile: water){
            glm::mat4 modelMatrix = Renderer::createTransformationMatrix(
                glm::vec3(tile.getX(), tile.getHeight(), tile.getZ()),
                0, 0, 0, tile.TILE_SIZE
            );
            shader.loadModelMatrix(modelMatrix);
            quad.Draw();
        }

        unbind();
    }

    void cleanUp(){
        shader.cleanUp();
    }
};

#endif