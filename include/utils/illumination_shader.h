#pragma once

using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/my_shader.h>
// #include <utils/utils.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


class IlluminationShader: public Shader {
private:
    const GLuint NR_LIGHTS = 1;

    GLint matDiffuseLocation;
    GLint matAmbientLocation;
    GLint matSpecularLocation;
    GLint kaLocation;
    GLint kdLocation;
    GLint ksLocation;
    GLint shineLocation;
    GLint alphaLocation;
    GLint f0Location;
    GLint projectionMatLocation;
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
    GLint normalMatrixLocation;
    vector<GLint> lightPosLocation;

public:
    void bindAttributes () {
        Shader::bindAttribute(0, "position");
        Shader::bindAttribute(1, "normal");
    }
    
    void getAllUniformLocations () {
        this->matDiffuseLocation = Shader::getUniformLocation("diffuseColor");
        this->matAmbientLocation = Shader::getUniformLocation("ambientColor");
        this->matSpecularLocation = Shader::getUniformLocation("specularColor");
        this->kaLocation = Shader::getUniformLocation("Ka");
        this->kdLocation = Shader::getUniformLocation("Kd");
        this->ksLocation = Shader::getUniformLocation("Ks");
        this->shineLocation = Shader::getUniformLocation("shininess");
        this->alphaLocation = Shader::getUniformLocation("alpha");
        this->f0Location = Shader::getUniformLocation("F0");
        this->projectionMatLocation = Shader::getUniformLocation("projectionMatrix");
        this->viewMatrixLocation = Shader::getUniformLocation("viewMatrix");
        this->modelMatrixLocation = Shader::getUniformLocation("modelMatrix");
        this->normalMatrixLocation = Shader::getUniformLocation("normalMatrix");
        
        for (GLuint i = 0; i < this->NR_LIGHTS; i++) {
            string number = to_string(i);
            this->lightPosLocation.push_back(Shader::getUniformLocation(("lights[" + number + "]").c_str()));
        }
    }

    void loadDiffuseColor (GLfloat diffColor []) {
        glUniform3fv(this->matDiffuseLocation, 1, diffColor);
    }

    void loadAmbientColor (GLfloat ambientColor []) {
        glUniform3fv(this->matAmbientLocation, 1, ambientColor);
    }

    void loadSpecularColor (GLfloat specColor []) {
        glUniform3fv(this->matSpecularLocation, 1, specColor);
    }

    void loadShine (GLfloat shininess) {
        glUniform1f(this->shineLocation, shininess);
    }

    void loadAlpha (GLfloat alpha) {
        glUniform1f(this->alphaLocation, alpha);
    }

    void loadF0 (GLfloat F0) {
        glUniform1f(this->f0Location, F0);
    }

    void loadKa (GLfloat ka) {
        glUniform1f(this->kaLocation, ka);
    }
    
    void loadKd (GLfloat kd) {
        glUniform1f(this->kdLocation, kd);
    }
    
    void loadKs (GLfloat ks) {
        glUniform1f(this->ksLocation, ks);
    }

    void loadLights (glm::vec3 lightPos []) {
        for (GLuint i = 0; i < NR_LIGHTS; i++) {
            glUniform3fv(this->lightPosLocation[i], 1, glm::value_ptr(lightPos[i]));
        }
    }

    void loadModelMatrix (glm::mat4 matrix) {
        glUniformMatrix4fv(this->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void loadNormalMatrix (glm::mat3 matrix) {
        glUniformMatrix3fv(this->normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void loadProjectionMatrix (glm::mat4 matrix){
        glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void loadViewMatrix(glm::mat4 matrix){
        glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    IlluminationShader():
    Shader("shaders/illumination.vert", "shaders/illumination.frag"){
        bindAttributes();

        glLinkProgram(this->program);
        glValidateProgram(this->program);

        getAllUniformLocations();

        Shader::checkCompileErrors(this->program, "PROGRAM");
    }

    
};