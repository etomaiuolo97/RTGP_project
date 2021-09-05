#pragma once

using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader {
protected:
    
    void checkCompileErrors (GLuint shader, string type) {
        GLint success;
        GLchar infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::"<< type <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
        }
        
    }

private:
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint loadShader (const GLchar* file, GLint type){
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
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, type == GL_VERTEX_SHADER? "VERTEX": "FRAGMENT");

        return shader;
    }

public:

    GLuint program;

    void getAllUniformLocations(){};

    GLint getUniformLocation(const GLchar* uniformName){
        return glGetUniformLocation(this->program, uniformName);
    }

    void bindAttributes (){};

    void bindAttribute (GLuint attribute, GLchar* varName) {
        glBindAttribLocation(this->program, attribute, varName);
    }

    Shader (const GLchar* vertexFile, const GLchar* fragmentFile) {
        this->vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
        this->fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

        this->program = glCreateProgram();

        glAttachShader(this->program, this->vertexShader);
        glAttachShader(this->program, this->fragmentShader);
    }

    void start () {
        glUseProgram(this->program);
    }

    void stop (){
        glUseProgram(0);
    }

    void cleanUp () {
        stop();
        glDetachShader(this->program, this->vertexShader);
        glDetachShader(this->program, this->fragmentShader);
        glDeleteShader(this->vertexShader);
        glDeleteShader(this->fragmentShader);
        glDeleteProgram(this->program);
    }

};