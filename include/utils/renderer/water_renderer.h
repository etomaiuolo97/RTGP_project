#pragma once

#ifndef WATER_RENDERER
#define WATER_RENDERER

#include "utils/renderer/renderer.h"
#include "utils/water_shader.h"
#include "utils/water_tile.h"
#include "utils/model.h"

class WaterRenderer : public Renderer{
private:
    WaterShader shader;
    Model quad;

    void prepareRender (Camera camera) {
        shader.start();
        shader.loadViewMatrix(camera.GetViewMatrix());
    }

    void unbind(){
        shader.stop();
    }

public:

    WaterRenderer (glm::mat4 projection)
        :Renderer(projection){        
        shader.start();
        shader.loadProjectionMatrix(projection);
        shader.stop();
        quad = Model("meshes/circle.obj");
    }

    void render(vector<WaterTile> water, Camera camera) {
        prepareRender(camera);

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