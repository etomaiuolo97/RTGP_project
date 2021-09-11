#pragma once

using namespace std;

#include <utils/shader.h>
#include <glm/gtc/type_ptr.hpp>


class particleShader : public Shader {

private:
    GLint projectionMatLocation;
    GLint viewMatrixLocation;


public:

    //GLuint program;

    //GLint getUniformLocation(const GLchar* uniformName){
      //  return glGetUniformLocation(this->program, uniformName);
    //}

    void getAllUniformLocations() {
        this->projectionMatLocation = Shader::getUniformLocation("projectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("viewMatrix");


        //viewMatrixLocation = getUniformLocation("viewMatrix");
        //projectionMatLocation = getUniformLocation("projectionMatrix");
    }

    void bindAttributes() {
        glCall(bindAttribute(0, "position"));
    }

    void loadViewMatrix(glm::mat4 modelView) {
        glCall(glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelView)));
    }

    void loadProjectionMatrix(glm::mat4 projectionMatrix) {
       glCall(glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)));
    }

    particleShader():Shader("../shaders/particle.vert", "../shaders/particle.frag"){
        bindAttributes();

        glLinkProgram(getProgram());
        glValidateProgram(getProgram());

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }

};