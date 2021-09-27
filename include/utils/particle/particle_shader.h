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
    GLint colorLocation;

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
        this->colorLocation = Shader::getUniformLocation("u_Color");
    }

    void bindAttributes() {
        Shader::bindAttribute(0,"a_Position");
    }
    
    void loadTransform(glm::mat4 transformation) {
        glCall(glUniformMatrix4fv(this->transformLocation, 1, GL_FALSE, glm::value_ptr(transformation)));
    }
    
    void loadColor(glm::vec4 color) {
        glCall(glUniform4fv(this->colorLocation, 1, glm::value_ptr(color)));
    }

    void loadView(glm::mat4 view) {
        glCall(glUniformMatrix4fv(this->viewLocation, 1, GL_FALSE, glm::value_ptr(view)));
    }

    void loadProjection(glm::mat4 projection) {
        glCall(glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, glm::value_ptr(projection)));
    }
};

#endif