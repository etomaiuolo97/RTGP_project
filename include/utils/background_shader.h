#pragma once

using namespace std;

#include <utils/shader.h>

class BackgroundShader: public Shader {
private:
    GLint projectionMatLocation;
    GLint viewMatrixLocation;
    GLint tCubeLocation;

public:
    void bindAttributes () {
        Shader::bindAttribute(0, "position");
    }
    
    void getAllUniformLocations () {
        this->projectionMatLocation = Shader::getUniformLocation("projectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("viewMatrix");
        this->tCubeLocation = Shader::getUniformLocation("tCube");
    }

    void loadtCube (GLint tCube) {
        glUniform1i(this->tCubeLocation, tCube);
    }

    void loadProjectionMatrix (glm::mat4 matrix){
        glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void loadViewMatrix(glm::mat4 matrix){
        glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    BackgroundShader():Shader("../shaders/background.vert", "../shaders/background.frag"){
        bindAttributes();

        glLinkProgram(getProgram());
        glValidateProgram(getProgram());

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }

    
};