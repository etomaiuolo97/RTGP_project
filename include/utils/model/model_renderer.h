#pragma once

#ifndef MODEL_RENDERER
#define MODEL_RENDERER

#include "utils/system/renderer.h"
#include "utils/model/model_shader.h"

class ModelRenderer : public Renderer{
private:
    ModelShader shader;

    vector<glm::vec3> lightPos = {glm::vec3(0.0f, 40.0f, 40.0f)};

    GLfloat Eta = 1.0f/1.52f;
    GLfloat mFresnelPower = 5.0f;

    vector<GLfloat> diffuseColor = {1.0f, 1.0f, 1.0f};
    vector<GLfloat> specularColor = {1.0f, 1.0f, 1.0f};
    vector<GLfloat> ambientColor = {0.8f, 0.8f, 0.8f};

    GLfloat Kd = 0.8f;
    GLfloat Ks = 0.5f;
    GLfloat Ka = 0.1f;

    GLfloat shininess = 25.0f;
    GLfloat alpha = 0.2f;
    GLfloat F0 = 0.9f;

    glm::vec3 position = glm::vec3(0.0f, -3.0f, -5.0f);

public:

    ModelRenderer (glm::mat4 projection)
        :Renderer(projection){
            Renderer::SetupShaders(shader.getProgram());
    }

    void render(Model& model, GLuint texture, Camera & camera, glm::vec4 clipPlane) {
        glm::mat4 view = Renderer::createViewMatrix(camera);

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

        glm::mat4 modMatrix = createTransformationMatrix(this->position, -90.0f, 0.0f, 0.0f, 0.8f);
        glm::mat4 norMatrix = createNormalMatrix(view, modMatrix);

        shader.loadModelMatrix(modMatrix);
        shader.loadNormalMatrix(norMatrix);

        model.Draw();

        shader.stop();
    }

    void cleanUp(){
        shader.cleanUp();
    }

    glm::vec3 getPosition() {
        return this->position;
    }

    vector<glm::vec3> getLightPos () {
        return this->lightPos;
    }

    vector<GLfloat> getLightColor () {
        return this->ambientColor;
    }

};

#endif