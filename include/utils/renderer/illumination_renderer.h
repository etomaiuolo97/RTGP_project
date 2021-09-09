#pragma once

#ifndef ILLUMINATION_RENDERER
#define ILLUMINATION_RENDERER

#include "utils/renderer/renderer.h"
#include "utils/illumination_shader.h"

class IlluminationRenderer : public Renderer{
private:
    IlluminationShader shader;

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

    IlluminationRenderer (Camera& camera, glm::mat4 projection)
        :Renderer(camera, projection){
            Renderer::SetupShaders(shader.getProgram());
    }

    void render(Model& model, GLuint texture, Camera camera) {
        glm::mat4 view = camera.GetViewMatrix();

        shader.start();

        Renderer::render(model, texture, shader.getProgram());

        shader.loadAmbientColor(ambientColor);
        shader.loadSpecularColor(specularColor);
        shader.loadShine(shininess);
        shader.loadAlpha(alpha);
        shader.loadF0(F0);

        shader.loadKa(0.0f);
        shader.loadKd(0.6f);
        shader.loadKs(0.0f);

        shader.loadLights(lightPos);

        shader.loadProjectionMatrix(this->projection);
        shader.loadViewMatrix(view);
        
        glCall(glActiveTexture(GL_TEXTURE0));
        glCall(glBindTexture(GL_TEXTURE_2D, texture));
        
        shader.loadRepeat(1.0f);
        shader.loadTex(0);

        shader.loadDiffuseColor(diffuseColor);

        glm::mat4 modMatrix = createTransformationMatrix(glm::vec3(0.0f, -3.0f, -5.0f), -90.0f, 0.0f, 0.0f, 0.8f);
        glm::mat4 norMatrix = createNormalMatrix(view, modMatrix);

        shader.loadModelMatrix(modMatrix);
        shader.loadNormalMatrix(norMatrix);

        model.Draw();

        shader.stop();
    }

    void cleanUp(){
        shader.cleanUp();
    }
};

#endif