#pragma once

#ifndef ILLUMINATION_SHADER
#define ILLUMINATION_SHADER

using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/shader.h>

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
    GLint repeatLocation;
    GLint texLocation;
    GLint planeLocation;
    vector<GLint> lightPosLocation;

public:

    IlluminationShader()
    :Shader("shaders/illumination.vert", "shaders/illumination.frag"){
        bindAttributes();

        glCall(glLinkProgram(getProgram()));
        glCall(glValidateProgram(getProgram()));

        getAllUniformLocations();

        Shader::checkCompileErrors(getProgram(), "PROGRAM");
    }


    void bindAttributes () {
        Shader::bindAttribute(0, "position");
        Shader::bindAttribute(1, "normal");
        Shader::bindAttribute(2, "UV");
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
        this->repeatLocation = Shader::getUniformLocation("repeat");
        this->texLocation = Shader::getUniformLocation("tex");
        this->planeLocation = Shader::getUniformLocation("plane");
        
        for (GLuint i = 0; i < this->NR_LIGHTS; i++) {
            string number = to_string(i);
            this->lightPosLocation.push_back(Shader::getUniformLocation(("lights[" + number + "]").c_str()));
        }
    }

    void loadRepeat (GLfloat repeat) {
        glCall(glUniform1f(this->repeatLocation, repeat));
    }

    void loadTex (GLint tex) {
        glCall(glUniform1i(this->texLocation, tex));
    }

    void loadDiffuseColor (vector<GLfloat> diffColor) {
        glCall(glUniform3fv(this->matDiffuseLocation, 1, diffColor.data()));
    }

    void loadAmbientColor (vector<GLfloat> ambientColor) {
        glCall(glUniform3fv(this->matAmbientLocation, 1, ambientColor.data()));
    }

    void loadSpecularColor (vector<GLfloat> specColor) {
        glCall(glUniform3fv(this->matSpecularLocation, 1, specColor.data()));
    }

    void loadShine (GLfloat shininess) {
        glCall(glUniform1f(this->shineLocation, shininess));
    }

    void loadAlpha (GLfloat alpha) {
        glCall(glUniform1f(this->alphaLocation, alpha));
    }

    void loadF0 (GLfloat F0) {
        glCall(glUniform1f(this->f0Location, F0));
    }

    void loadKa (GLfloat ka) {
        glCall(glUniform1f(this->kaLocation, ka));
    }
    
    void loadKd (GLfloat kd) {
        glCall(glUniform1f(this->kdLocation, kd));
    }
    
    void loadKs (GLfloat ks) {
        glCall(glUniform1f(this->ksLocation, ks));
    }

    void loadLights (vector<glm::vec3> lightPos) {
        for (GLuint i = 0; i < NR_LIGHTS; i++) {
            glCall(glUniform3fv(this->lightPosLocation[i], 1, glm::value_ptr(lightPos[i])));
        }
    }

    void loadPlane (glm::vec4 plane) {
        glCall(glUniform4f(this->planeLocation, plane.x, plane.y, plane.z, plane.w));
    }

    void loadModelMatrix (glm::mat4 matrix) {
        glCall(glUniformMatrix4fv(this->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadNormalMatrix (glm::mat3 matrix) {
        glCall(glUniformMatrix3fv(this->normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadProjectionMatrix (glm::mat4 matrix){
        glCall(glUniformMatrix4fv(this->projectionMatLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadViewMatrix(glm::mat4 matrix){
        glCall(glUniformMatrix4fv(this->viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
    }  
};

#endif