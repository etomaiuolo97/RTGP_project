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
    GLint projectionMatLocation;
    GLint reflectionTextureLocation;
    GLint refractionTextureLocation;
    GLint dudvMapLocation;
    GLint moveFactorLocation;
    GLint cameraPositionLocation;
    GLint normalMapLocation;
    GLint lightColourLocation;
    GLint lightPositionLocation;
    GLint depthMapLocation;

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
        Shader::bindAttribute(0, "position");
    }
    
    void getAllUniformLocations () {
        this->projectionMatLocation = Shader::getUniformLocation("projectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("viewMatrix");
        this->modelMatrixLocation = Shader::getUniformLocation("modelMatrix");
        this->reflectionTextureLocation = Shader::getUniformLocation("reflectionTexture");
        this->refractionTextureLocation = Shader::getUniformLocation("refractionTexture");
        this->dudvMapLocation = Shader::getUniformLocation("dudvMap");
        this->moveFactorLocation = Shader::getUniformLocation("moveFactor");
        this->cameraPositionLocation = Shader::getUniformLocation("cameraPosition");
        this->normalMapLocation = Shader::getUniformLocation("normalMap");
        this->lightColourLocation = Shader::getUniformLocation("lightColour");
        this->lightPositionLocation = Shader::getUniformLocation("lightPosition");
        this->depthMapLocation = Shader::getUniformLocation("depthMap");
    }

    void connectTextureUnits () {
        glCall(glUniform1i(this->reflectionTextureLocation, 0));
        glCall(glUniform1i(this->refractionTextureLocation, 1));
        glCall(glUniform1i(this->dudvMapLocation, 2));
        glCall(glUniform1i(this->normalMapLocation, 3));
        glCall(glUniform1i(this->depthMapLocation, 4));
    }

    void loadLight (glm::vec3 lightPosition, glm::vec3 lightColour){
        glCall(glUniform3fv(this->lightPositionLocation, 1, glm::value_ptr(lightPosition)));
        glCall(glUniform3fv(this->lightColourLocation, 1, glm::value_ptr(lightColour)));
    }

    void loadCameraPosition (glm::vec3 cameraPos){
        glCall(glUniform3fv(this->cameraPositionLocation, 1, glm::value_ptr(cameraPos)));
    }

    void loadMoveFactor (GLfloat factor) {
        glCall(glUniform1f(this->moveFactorLocation, factor));
    }

    void loadModelMatrix (glm::mat4 matrix) {
        glCall(glUniformMatrix4fv(this->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadProjectionMatrix (glm::mat4 matrix){
        glCall(glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadViewMatrix(glm::mat4 matrix){
        glCall(glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }  
};

#endif