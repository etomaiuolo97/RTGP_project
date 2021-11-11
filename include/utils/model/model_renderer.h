#pragma once

#ifndef MODEL_RENDERER
#define MODEL_RENDERER

#include "utils/system/renderer.h"
#include "utils/model/model_shader.h"

class ModelRenderer : public Renderer{
private:
    ModelShader shader;
    Light light;
    glm::vec3 position = {0.0f, -3.0f, -5.0f};

public:

    ModelRenderer (glm::mat4 projection, bool night)
        : Renderer(projection){
        
        light.position = glm::vec3(-10.0f, 20.0f, -60.0f);

        if(night)
            light.color = glm::vec3(0.6f, 0.6f, 0.6f);
        else
            light.color = glm::vec3(1.0f, 1.0f, 1.0f);

        shader.start();
        shader.loadProjectionMatrix(this->projection);
        shader.stop();

        Renderer::SetupShaders(shader.getProgram());
    }

    void render(Model& model, Texture& texture, Camera & camera, glm::vec4 clipPlane) {
        glm::mat4 view = Renderer::createViewMatrix(camera);

        shader.start();

        shader.loadLight(this->light);
        shader.loadViewMatrix(view);
        shader.loadClipPlane(clipPlane);

        glCall(glActiveTexture(GL_TEXTURE0));
        glCall(glBindTexture(GL_TEXTURE_2D, texture.id));
        
        shader.loadTexture(texture);
        shader.loadModelMatrix(Renderer::createTransformationMatrix(position, -90.0f, 0.0f, 0.0f, 1.0f));

        model.Draw();

        shader.stop();
    }

    void cleanUp(){
        shader.cleanUp();
    }

    glm::vec3 getPosition() {
        return this->position;
    }

    glm::vec3 getLightPos () {
        return this->light.position;
    }

    glm::vec3 getLightColor () {
        return this->light.color;
    }

    Light getLight () {
        return this->light;
    }

};

#endif