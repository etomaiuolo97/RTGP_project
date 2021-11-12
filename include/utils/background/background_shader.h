#pragma once

#ifndef BACKGROUND_SHADER
#define BACKGROUND_SHADER

#include <utils/system/shader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class BackgroundShader: public Shader {
private:
    GLint projectionMatLocation;
    GLint viewMatrixLocation;
    GLint tCubeLocation;
    /*
    GLint tCube2Location;
    
    GLint blendFactorLocation;
    GLint fogColourLocation;

    float ROTATE_SPEED = 1.0f;
    float rotation = 0;
    */

public:
    void bindAttributes () {
        Shader::bindAttribute(0, "position");
    }
    
    void getAllUniformLocations () {
        this->projectionMatLocation = Shader::getUniformLocation("projectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("viewMatrix");
        this->tCubeLocation = Shader::getUniformLocation("tCube");
        /*
        this->tCube2Location = Shader::getUniformLocation("tCube2");
        this->blendFactorLocation = Shader::getUniformLocation("blendFactor");  
        this->fogColourLocation = Shader::getUniformLocation("fogColour");
        */
    }

/*
    void loadBlendFactor (float blendFactor) {
        glUniform1i(this->blendFactorLocation, blendFactor);
    }

    void connectTextureUnits() {
        glUniform1i(this->tCubeLocation, 0);
        glUniform1i(this->tCube2Location, 1);
    }

    void loadFogColour (float r, float g, float b) {
        glm::vec3 fogColour = glm::vec3(r,g,b);
        glCall(glUniform3fv(this->fogColourLocation, 1, glm::value_ptr(fogColour)));
    }
    */

    void loadtCube (GLint tCube) {
        glUniform1i(this->tCubeLocation, tCube);
    }

    void loadProjectionMatrix (glm::mat4 matrix){
        glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void loadViewMatrix(glm::mat4 matrix){
        glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    BackgroundShader():Shader("shaders/background.vert", "shaders/background.frag"){
        bindAttributes();

        glLinkProgram(getProgram());
        glValidateProgram(getProgram());

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }

    
};

#endif