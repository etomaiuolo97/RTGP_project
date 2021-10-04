#pragma once

#ifndef PARTICLE_SHADER
#define PARTICLE_SHADER

#include <glm/gtc/type_ptr.hpp>

#include <utils/system/shader.h>

class ParticleShader : public Shader {

private:
    GLint viewLocation;
    GLint projectionLocation;
    GLint transformLocation;
    GLint reflectionTextureLocation;
    GLint refractionTextureLocation;
    GLint depthMapLocation;
    GLint cameraPositionLocation;
    GLint lightPositionLocation;

public:

    ParticleShader() : 
    Shader("./shaders/particle.vert", "./shaders/particle.frag"){
        bindAttributes();
        
        glCall(glLinkProgram(getProgram()));
        glCall(glValidateProgram(getProgram()));

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }


    void getAllUniformLocations() {
        this->viewLocation = Shader::getUniformLocation("u_View");
        this->projectionLocation = Shader::getUniformLocation("u_Projection");
        this->transformLocation = Shader::getUniformLocation("u_Transform");
        this->reflectionTextureLocation = Shader::getUniformLocation("u_ReflectionTexture");
        this->refractionTextureLocation = Shader::getUniformLocation("u_RefractionTexture");
        this->depthMapLocation = Shader::getUniformLocation("u_DepthMap");
        this->cameraPositionLocation = Shader::getUniformLocation("u_CameraPosition");
        this->lightPositionLocation = Shader::getUniformLocation("u_LightPosition");
    }

    void connectTextureUnits() {
        glCall(glUniform1i(this->reflectionTextureLocation, 0));
        glCall(glUniform1i(this->refractionTextureLocation, 1));
        glCall(glUniform1i(this->depthMapLocation, 2));
    }

    void bindAttributes() {
        Shader::bindAttribute(0,"a_Position");
        Shader::bindAttribute(1,"a_Normal");
    }
    
    void loadTransform(glm::mat4 transformation) {
        glCall(glUniformMatrix4fv(this->transformLocation, 1, GL_FALSE, glm::value_ptr(transformation)));
    }
    
    void loadLight (Light light) {
        glCall(glUniform3fv(this->lightPositionLocation, 1, glm::value_ptr(light.position)));
    }

    void loadLight (glm::vec3 cameraPos) {
        glCall(glUniform3fv(this->cameraPositionLocation, 1, glm::value_ptr(cameraPos)));
    }

    void loadView(glm::mat4 view) {
        glCall(glUniformMatrix4fv(this->viewLocation, 1, GL_FALSE, glm::value_ptr(view)));
    }

    void loadProjection(glm::mat4 projection) {
        glCall(glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, glm::value_ptr(projection)));
    }
};

#endif