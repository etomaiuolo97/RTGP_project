#pragma once

using namespace std;

#include <utils/shader.h>
#include <glm/gtc/type_ptr.hpp>


class particleShader : public Shader {

private:
    GLint projectionMatLocation;
    GLint viewMatrixLocation;


public:

    GLuint program;

    GLint getUniformLocation(const GLchar* uniformName){
        return glGetUniformLocation(this->program, uniformName);
    }

    void getAllUniformLocations() {
        viewMatrixLocation = getUniformLocation("viewMatrix");
        projectionMatLocation = getUniformLocation("projectionMatrix");
    }

    void bindAttributes() {
        bindAttribute(0, "position");
    }

    void loadViewMatrix(glm::mat4 modelView) {
            glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelView));
    }

    void loadProjectionMatrix(glm::mat4 projectionMatrix) {
            glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    particleShader():Shader("particle.vert", "particle.frag"){
        bindAttributes();

        glLinkProgram(this->program);
        glValidateProgram(this->program);

        getAllUniformLocations();

        Shader::checkCompileErrors(this->program, "PROGRAM");
    }

};