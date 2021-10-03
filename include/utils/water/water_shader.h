#pragma once

#ifndef WATER_SHADER
#define WATER_SHADER

#include <utils/system/shader.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


class WaterShader: public Shader {
private:
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
    GLint projectionMatrixLocation;
    GLint reflectionTextureLocation;
    GLint refractionTextureLocation;
    GLint dudvMapLocation;
    GLint moveFactorLocation;
    GLint cameraPositionLocation;

public:

    WaterShader()
    :Shader("./shaders/water.vert", "./shaders/water.frag"){
        bindAttributes();

        glCall(glLinkProgram(getProgram()));
        glCall(glValidateProgram(getProgram()));

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }


    void bindAttributes () {
        Shader::bindAttribute(0, "a_Position");
    }
    
    void getAllUniformLocations () {
        this->projectionMatrixLocation = Shader::getUniformLocation("u_ProjectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("u_ViewMatrix");
        this->modelMatrixLocation = Shader::getUniformLocation("u_ModelMatrix");
        this->reflectionTextureLocation = Shader::getUniformLocation("u_ReflectionTexture");
        this->refractionTextureLocation = Shader::getUniformLocation("u_RefractionTexture");
        this->dudvMapLocation = Shader::getUniformLocation("u_DuDvMap");
        this->moveFactorLocation = Shader::getUniformLocation("u_MoveFactor");
        this->cameraPositionLocation = Shader::getUniformLocation("u_CameraPosition");
        // this->normalMapLocation = Shader::getUniformLocation("normalMap");
        // this->lightColourLocation = Shader::getUniformLocation("lightColour");
        // this->lightPositionLocation = Shader::getUniformLocation("lightPosition");
        // this->depthMapLocation = Shader::getUniformLocation("depthMap");
    }

    void connectTextureUnits () {
        glCall(glUniform1i(this->reflectionTextureLocation, 0));
        glCall(glUniform1i(this->refractionTextureLocation, 1));
        glCall(glUniform1i(this->dudvMapLocation, 2));
        // glCall(glUniform1i(this->normalMapLocation, 3));
        // glCall(glUniform1i(this->depthMapLocation, 4));
    }

    void loadLight (glm::vec3 lightPosition, glm::vec3 lightColour){
        // glCall(glUniform3fv(this->lightPositionLocation, 1, glm::value_ptr(lightPosition)));
        // glCall(glUniform3fv(this->lightColourLocation, 1, glm::value_ptr(lightColour)));
    }

    void loadCameraPosition (glm::vec3 cameraPos){
        glCall(glUniform3fv(this->cameraPositionLocation, 1, glm::value_ptr(cameraPos)));
    }

    void loadMoveFactor (GLfloat factor) {
        glCall(glUniform1f(this->moveFactorLocation, factor));
    }

    void loadModelMatrix (glm::mat4& matrix) {
        glCall(glUniformMatrix4fv(this->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadProjectionMatrix (glm::mat4& matrix){
        glCall(glUniformMatrix4fv(this->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadViewMatrix(glm::mat4& matrix){
        glCall(glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }  
};

#endif