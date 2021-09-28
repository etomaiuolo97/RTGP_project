#pragma once

#ifndef PARTICLE_SHADER
#define PARTICLE_SHADER

#include <glm/gtc/type_ptr.hpp>

#include <utils/system/shader.h>

class ParticleShader : public Shader {

private:
    GLint viewLocation;
    GLint projectionLocation;
    GLint transformLocation;
    GLint texOffset1Location;
    GLint texOffset2Location;
    GLint texCoordsInfoLocation;

public:

    ParticleShader() : 
    Shader("./shaders/particle.vert", "./shaders/particle.frag"){
        bindAttributes();
        
        glCall(glLinkProgram(getProgram()));
        glCall(glValidateProgram(getProgram()));

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }


    void getAllUniformLocations() {
        this->viewLocation = Shader::getUniformLocation("u_View");
        this->projectionLocation = Shader::getUniformLocation("u_Projection");
        this->transformLocation = Shader::getUniformLocation("u_Transform");
        this->texOffset1Location = Shader::getUniformLocation("texOffset1");
        this->texOffset2Location = Shader::getUniformLocation("texOffset2");
        this->texCoordsInfoLocation = Shader::getUniformLocation("textureCoordsInfo");
    }

    void bindAttributes() {
        Shader::bindAttribute(0,"a_Position");
    }
    
    void loadTransform(glm::mat4 transformation) {
        glCall(glUniformMatrix4fv(this->transformLocation, 1, GL_FALSE, glm::value_ptr(transformation)));
    }
    
    void loadTexture(glm::vec2 offset1, glm::vec2 offset2, GLfloat numRows, GLfloat blend) {
        glCall(glUniform2fv(this->texOffset1Location, 1, glm::value_ptr(offset1)));
        glCall(glUniform2fv(this->texOffset2Location, 1, glm::value_ptr(offset2)));
        glCall(glUniform2fv(this->texCoordsInfoLocation, 1, glm::value_ptr(glm::vec2(numRows, blend))));
    }

    void loadView(glm::mat4 view) {
        glCall(glUniformMatrix4fv(this->viewLocation, 1, GL_FALSE, glm::value_ptr(view)));
    }

    void loadProjection(glm::mat4 projection) {
        glCall(glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, glm::value_ptr(projection)));
    }
};

#endif