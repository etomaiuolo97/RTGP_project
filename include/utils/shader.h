#pragma once

#ifndef SHADER
#define SHADER

using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "utils/utils.h"

class Shader {
protected:
    
    void checkCompileErrors (GLuint shader, string type) {
        GLint success;
        GLchar infoLog[1024];

        if (type != "PROGRAM") {
            glCall(glGetShaderiv (shader, GL_COMPILE_STATUS, &success));
            if (!success) {
                glCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
                std::cout << "ERROR::SHADER::"<< type <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }
        else {
            glCall(glGetProgramiv(shader, GL_LINK_STATUS, &success));
            if (!success) {
                glCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
        }
        
    }

private:
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint program;

    GLuint loadShader (const GLchar* file, GLint type){
        std::cout << file << std::endl;
        string code;
        ifstream shaderFile;

        shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        
        try{
            shaderFile.open(file);
            
            stringstream shaderStream;

            shaderStream << shaderFile.rdbuf();

            shaderFile.close();

            code = shaderStream.str();
        }
        catch (ifstream::failure e) {
            cout << "ERROR :: SHADER :: FILE_NOT_SUCCESSFULLY_READ" << endl;
        }

        const GLchar* shaderCode = code.c_str();

        GLuint shader;

        shader = glCreateShader(type);
        glCall(glShaderSource(shader, 1, &shaderCode, NULL));
        glCall(glCompileShader(shader));
        checkCompileErrors(shader, type == GL_VERTEX_SHADER? "VERTEX": "FRAGMENT");

        return shader;
    }

public:    

    // Delete copy constructor and copy assignment
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& move) {
        return *(this);
    };

    // Default move constructor and move assignment
    Shader& operator=(Shader&& move) noexcept = default;
    Shader(Shader&& shader) = default;

    void getAllUniformLocations(){};

    GLint getUniformLocation(const GLchar* uniformName){
        return glGetUniformLocation(this->program, uniformName);
    }

    void bindAttributes (){};

    void bindAttribute (GLuint attribute, GLchar* varName) {
        glCall(glBindAttribLocation(this->program, attribute, varName));
    }

    Shader (const GLchar* vertexFile, const GLchar* fragmentFile) {
        this->vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
        this->fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

        this->program = glCreateProgram();

        glCall(glAttachShader(this->program, this->vertexShader));
        glCall(glAttachShader(this->program, this->fragmentShader));
    }

    GLuint getProgram(){
        return this->program;
    }

    void setProgram (GLuint program)  {
        this->program = program;
    }

    void start () {
        glCall(glUseProgram(this->program));
    }

    void stop (){
        glCall(glUseProgram(0));
    }

    void cleanUp () {
        stop();
        glCall(glDetachShader(this->program, this->vertexShader));
        glCall(glDetachShader(this->program, this->fragmentShader));
        glCall(glDeleteShader(this->vertexShader));
        glCall(glDeleteShader(this->fragmentShader));
        glCall(glDeleteProgram(this->program));
    }

};

#endif