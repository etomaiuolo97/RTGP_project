#pragma once

using namespace std;

#ifndef PARTICLE_SHADER
#define PARTICLE_SHADER

#include <utils/shader.h>
#include <glm/gtc/type_ptr.hpp>


class particleShader : public Shader {

private:
    GLint projectionMatLocation;
    GLint viewMatrixLocation;
    //GLint textOffset1Location;
    //GLint textOffset2Location;
    //GLint textCoordInfoLocation;
    GLint numberRowsLocation;


public:

    particleShader():Shader("../shaders/particle.vert", "../shaders/particle.frag"){
        bindAttributes();

        glCall(glLinkProgram(getProgram()));
        glCall(glValidateProgram(getProgram()));

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }


    void getAllUniformLocations() {
        this->projectionMatLocation = Shader::getUniformLocation("projectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("viewMatrix");
        //this->textOffset1Location = Shader::getUniformLocation("texOffset1");
        //this->textOffset2Location = Shader::getUniformLocation("texOffset2");
        //this->textCoordInfoLocation = Shader::getUniformLocation("textCoordInfo");
        this->numberRowsLocation = Shader::getUniformLocation("numberRows");


    }

    void bindAttributes() {
        Shader::bindAttribute(0,"position");
    }
    
    void loadViewMatrix(glm::mat4 modelView) {
        glCall(glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelView)));
    }
    

    void loadProjectionMatrix(glm::mat4 projectionMatrix) {
       glCall(glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)));
    }

/*
    void loadTextureCoordInfo(glm::vec2 offset1, glm::vec2 offset2, float numRows, float blend){
        glCall(glUniform2f(this->textOffset1Location,offset1.x,offset1.y));
        glCall(glUniform2f(this->textOffset2Location,offset2.x,offset2.y));
        glCall(glUniform2f(this->textOffset1Location,numRows,blend));
        
    }
*/

    void loadNumberRows(float numberRows){
        glCall(glUniform1f(numberRowsLocation,numberRows));
    }

};

#endif