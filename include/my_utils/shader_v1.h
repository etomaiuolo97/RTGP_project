#pragma once

using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        GLuint Program;

        Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
        {
            string vertexCode;
            string fragmentCode;
            ifstream vShaderFile;
            ifstream fShaderFile;

            vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            try{
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);

                stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (ifstream::failure e) {
                cout << "ERROR!" << endl;
            }

            const GLchar* vShaderCode = vertexCode.c_str();
            const GLchar* fShaderCode = fragmentCode.c_str();

            GLuint vertex, fragment;

            // Vertex shader = label representing the obj
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

            // Fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            // Link shaders
            this->Program = glCreateProgram();
            glAttachShader(this->Program, vertex);
            glAttachShader(this->Program, fragment);
            glLinkProgram(this->Program);
            checkCompileErrors(this->Program, "PROGRAM");
            
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void Use()
        {
            glUseProgram(this->Program);
        }

        void Delete()
        {
            glDeleteProgram(this->Program);
        }
        
    private:
        void checkCompileErrors(GLuint shader, string type)
        {
            GLint success;
            GLchar infoLog[512];
            
            if(type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                }
            }
            
        }
     
};