#pragma once

#ifndef GUI_SHADER
#define GUI_SHADER

#include <utils/system/shader.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


class GuiShader: public Shader {
private:
    GLint modelMatrixLocation;
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
        Shader::bindAttribute(0, "a_Position");
        Shader::bindAttribute(2, "a_TextureCoords");
    }
    
    void getAllUniformLocations () {
        this->modelMatrixLocation = Shader::getUniformLocation("u_ModelMatrix");
        this->guiTextureLocation = Shader::getUniformLocation("u_GuiTexture");
    }

    void combineTextures () {
        glCall(glUniform1i(this->guiTextureLocation, 0));
    }

    void loadModelMatrix (glm::mat4 matrix) {
        glCall(glUniformMatrix4fv(this->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
};

#endif