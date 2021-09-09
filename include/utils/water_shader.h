#pragma once

#ifndef WATER_SHADER
#define WATER_SHADER

#include <utils/shader.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


class WaterShader: public Shader {
private:
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
    GLint projectionMatLocation;

public:

    WaterShader()
    :Shader("shaders/water.vert", "shaders/water.frag"){
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