#pragma once

#ifndef GUI_SHADER
#define GUI_SHADER

#include <utils/shader.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


class GuiShader: public Shader {
private:
    GLint transformationMatrixLocation;

public:

    GuiShader()
    :Shader("../shaders/gui.vert", "../shaders/gui.frag"){
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
    }

    void loadTansformationMatrix (glm::mat4 matrix) {
        glCall(glUniformMatrix4fv(this->transformationMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
};

#endif