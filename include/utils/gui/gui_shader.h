#pragma once

#ifndef GUI_SHADER
#define GUI_SHADER

#include <utils/system/shader.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


class GuiShader: public Shader {
private:
    GLint transformationMatrixLocation;
    GLint cameraPositionLocation;
    GLint lightPositionLocation;
    GLint guiTextureLocation;

public:

    GuiShader()
    :Shader("shaders/gui.vert", "shaders/gui.frag"){
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
        this->transformationMatrixLocation = Shader::getUniformLocation("transformationMatrix");
        this->cameraPositionLocation = Shader::getUniformLocation("u_CameraPosition");
        this->lightPositionLocation = Shader::getUniformLocation("u_LightPosition");
        this->guiTextureLocation = Shader::getUniformLocation("guiTexture");
    }

    void combineTextures () {
        glCall(glUniform1i(this->guiTextureLocation, 0));
    }

    void loadCameraPosition (glm::vec3 cameraPos) {
        glCall(glUniform3fv(this->cameraPositionLocation, 1, glm::value_ptr(cameraPos)));
    }

    void loadLightPosition (glm::vec3 lightPos) {
        glCall(glUniform3fv(this->lightPositionLocation, 1, glm::value_ptr(lightPos)));
    }

    void loadTansformationMatrix (glm::mat4 matrix) {
        glCall(glUniformMatrix4fv(this->transformationMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
};

#endif