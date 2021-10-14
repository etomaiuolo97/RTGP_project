#pragma once

#ifndef MODEL_SHADER
#define MODEL_SHADER

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/system/shader.h>
#include <utils/system/utils.h>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

struct Texture {
    GLint id;

    GLfloat shineDamper = 1.0f;
    GLfloat reflectivity = 0.0f;
};

class ModelShader: public Shader {
private:
    GLint modelMatrixLocation;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
    GLint lightPositionLocation;
    GLint lightColorLocation;
    GLint shineDamperLocation;
    GLint reflectivityLocation;
    GLint clipPlaneLocation;

public:

    ModelShader()
    :Shader("shaders/model.vert", "shaders/model.frag"){
        bindAttributes();

        glCall(glLinkProgram(getProgram()));
        glCall(glValidateProgram(getProgram()));

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }


    void bindAttributes () {
        Shader::bindAttribute(0, "a_Position");
        Shader::bindAttribute(1, "a_Normal");
        Shader::bindAttribute(2, "a_TextureCoords");
    }
    
    void getAllUniformLocations () {
        this->modelMatrixLocation = Shader::getUniformLocation("u_ModelMatrix");
        this->projectionMatrixLocation = Shader::getUniformLocation("u_ProjectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("u_ViewMatrix");
        this->lightPositionLocation = Shader::getUniformLocation("u_LightPosition");
        this->lightColorLocation = Shader::getUniformLocation("u_LightColor");
        this->shineDamperLocation = Shader::getUniformLocation("u_ShineDamper");
        this->reflectivityLocation = Shader::getUniformLocation("u_Reflectivity");
        this->clipPlaneLocation = Shader::getUniformLocation("u_ClipPlane");
    }

    void loadClipPlane (glm::vec4& clipPlane) {
        glCall(glUniform4fv(this->clipPlaneLocation, 1, glm::value_ptr(clipPlane)));
    }

    void loadLight (Light& light) {
        glCall(glUniform3fv(this->lightPositionLocation, 1, glm::value_ptr(light.position)));
        glCall(glUniform3fv(this->lightColorLocation, 1, glm::value_ptr(light.color)));
    }

    void loadTexture (Texture& texture) {
        glCall(glUniform1f(this->shineDamperLocation, texture.shineDamper));
        glCall(glUniform1f(this->reflectivityLocation, texture.reflectivity));
    }

    void loadModelMatrix (glm::mat4 matrix) {
        glCall(glUniformMatrix4fv(this->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadProjectionMatrix (glm::mat4 matrix){
        glCall(glUniformMatrix4fv(this->projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadViewMatrix(glm::mat4 matrix){
        glCall(glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }  
};

#endif