#pragma once

#ifndef PARTICLE_SHADER
#define PARTICLE_SHADER

#include <glm/gtc/type_ptr.hpp>

#include <utils/system/shader.h>

class ParticleShader : public Shader {

private:
    GLint projectionLocation;
    GLint offsetLocation;
    GLint colorLocation;
    GLint spriteLocation;

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
        this->projectionLocation = Shader::getUniformLocation("projection");
        this->offsetLocation = Shader::getUniformLocation("offset");
        this->colorLocation = Shader::getUniformLocation("color");
        this->spriteLocation = Shader::getUniformLocation("sprite");
    }

    void bindAttributes() {
        Shader::bindAttribute(0,"vertex");
    }
    
    void loadOffset(glm::vec2 offset) {
        glCall(glUniform2fv(this->offsetLocation, 1, glm::value_ptr(offset)));
    }
    
    void loadColor(glm::vec4 color) {
        glCall(glUniform4fv(this->colorLocation, 1, glm::value_ptr(color)));
    }

    void loadProjection(glm::mat4 projectionMatrix) {
       glCall(glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)));
    }

    void loadSprite(GLint sprite) {
        glCall(glUniform1i(this->spriteLocation, sprite));
    }
};

#endif