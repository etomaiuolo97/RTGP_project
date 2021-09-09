#pragma once

#ifndef BACKGROUND_RENDERER
#define BACKGROUND_RENDERER

#include "utils/renderer/renderer.h"
#include "utils/background_shader.h"

class BackgroundRenderer : public Renderer{
private:
    BackgroundShader shader;

    glm::vec3 lightPos [1] = {glm::vec3(0.0f, 0.0f, 10.0f)};

    GLfloat Eta = 1.0f/1.52f;
    GLfloat mFresnelPower = 5.0f;

    GLfloat diffuseColor [3] = {1.0f, 1.0f, 1.0f};
    GLfloat specularColor [3] = {1.0f, 1.0f, 1.0f};
    GLfloat ambientColor [3] = {0.1f, 0.1f, 0.1f};

    GLfloat Kd = 0.5f;
    GLfloat Ks = 0.4f;
    GLfloat Ka = 0.1f;

    GLfloat shininess = 25.0f;
    GLfloat alpha = 0.2f;
    GLfloat F0 = 0.9f;

public:

    BackgroundRenderer (glm::mat4 projection)
        :Renderer(projection){
            Renderer::SetupShaders(shader.getProgram());
    }

    void render(Model& model, GLuint texture, Camera camera) {
        glm::mat4 view = camera.GetViewMatrix();

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