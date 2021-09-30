#pragma once

#ifndef BACKGROUND_RENDERER
#define BACKGROUND_RENDERER

#include "utils/system/renderer.h"
#include "utils/background/background_shader.h"

class BackgroundRenderer : public Renderer{
private:
    BackgroundShader shader;

public:

    BackgroundRenderer (glm::mat4 projection)
        :Renderer(projection){
            Renderer::SetupShaders(shader.getProgram());
    }

    void render(Model& model, GLuint texture, Camera & camera) {
        glm::mat4 view = Renderer::createViewMatrix(camera);

        glDepthFunc(GL_LEQUAL);

        shader.start();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        shader.loadProjectionMatrix(projection);
        
        view = glm::mat4(glm::mat3(view));
        shader.loadViewMatrix(view);
        
        shader.loadtCube(0);

        model.Draw();
        shader.stop();

        glDepthFunc(GL_LESS);
    }

    void cleanUp(){
        shader.cleanUp();
    }
};

#endif