#pragma once

#ifndef ILLUMINATION_RENDERER
#define ILLUMINATION_RENDERER

#include "utils/system/renderer.h"
#include "utils/illumination/illumination_shader.h"

class IlluminationRenderer : public Renderer{
private:
    IlluminationShader shader;

    vector<glm::vec3> lightPos = {glm::vec3(0.0f, 0.0f, 10.0f)};

    GLfloat Eta = 1.0f/1.52f;
    GLfloat mFresnelPower = 5.0f;

    vector<GLfloat> diffuseColor = {1.0f, 1.0f, 1.0f};
    vector<GLfloat> specularColor = {1.0f, 1.0f, 1.0f};
    vector<GLfloat> ambientColor = {0.1f, 0.1f, 0.1f};

    GLfloat Kd = 0.5f;
    GLfloat Ks = 0.4f;
    GLfloat Ka = 0.1f;

    GLfloat shininess = 25.0f;
    GLfloat alpha = 0.2f;
    GLfloat F0 = 0.9f;

public:

    IlluminationRenderer (glm::mat4 projection)
        :Renderer(projection){
            Renderer::SetupShaders(shader.getProgram());
    }

    void render(Model& model, GLuint texture, Camera & camera, glm::vec4 clipPlane) {
        glm::mat4 view = camera.GetViewMatrix();

        shader.start();

        Renderer::render(shader.getProgram());

        shader.loadPlane(clipPlane);
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

    vector<glm::vec3> getLightPos () {
        return this->lightPos;
    }

    vector<GLfloat> getLightColor () {
        return this->ambientColor;
    }

};

#endif