#pragma once

#ifndef RENDERER
#define RENDERER

#include <iostream>
#include <vector>

#include <utils/model/model.h>

#include <utils/system/camera.h>
#include <utils/system/shader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Renderer {
protected:
    Camera camera;
    glm::mat4 projection;

    // Vector for all the Shader Programs used and swapped in the application
    vector<std::string> shaders;

    // Index of the currente shader (= 0  in the beginning)
    GLuint current_subroutine = 0;

    /**
     * @brief Create a Normal Matrix object
     * 
     * @param view 
     * @param modelMatrix 
     * @return glm::mat3 
     */
    glm::mat3 createNormalMatrix (glm::mat4 view, glm::mat4 modelMatrix) {
        glm::mat3 matrix = glm::mat3(1.0f);
        glm::inverseTranspose(glm::mat3(view * modelMatrix));
        return matrix;
    }

    /**
     * @brief Create a Transformation Matrix object. In this method we initialize a matrix that will be translated 
     * by the coordinates that indicates the position of the object, and then rotate along the radians given in 
     * input. Finally, it is scaled and returned.
     * 
     * @param translation 
     * @param rx 
     * @param ry 
     * @param rz 
     * @param scale 
     * @return glm::mat4 
     */
    glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, GLfloat scale) {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, translation);
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

        return matrix;
    }

    /**
     * @brief Create a Transformation Matrix object
     * 
     * @param translation 
     * @param scale 
     * @return glm::mat4 
     */
    glm::mat4 createTransformationMatrix(glm::vec2 translation, glm::vec2 rotation, glm::vec2 scale) {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, glm::vec3(translation.x, translation.y, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, glm::vec3(scale.x, scale.y, 1.0f));

        return matrix;
    }

    /**
     * @brief Create a View Matrix object
     * 
     * @param camera 
     * @return glm::mat4 
     */
    glm::mat4 createViewMatrix(Camera& camera) {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 cameraPos = camera.getPosition();
        glm::vec3 negCameraPos = glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
        matrix = glm::translate(matrix, negCameraPos);

        return matrix;
    }

    /**
     * @brief This function subroutines parameters of the system and the uniforms. With a loop we activate a 
     * subroutine calling by the name of the uniform. we calculate the number of subroutines and then with 
     * another loop we add the subroutine to the shader vector.
     * 
     * @param program 
     */
    void SetupShaders(int program) {
        int maxSub,maxSubU,countActiveSU;
        GLchar name[256];
        int len, numCompS;

        // Subroutines parameters of the system
        glCall(glGetIntegerv(GL_MAX_SUBROUTINES, &maxSub));
        glCall(glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &maxSubU));
        std::cout << "Max Subroutines:" << maxSub << " - Max Subroutine Uniforms:" << maxSubU << std::endl;

        // Subroutine uniforms 
        glCall(glGetProgramStageiv(program, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &countActiveSU));

        for (int i = 0; i < countActiveSU; i++) {

            // Name of subroutine uniform
            glCall(glGetActiveSubroutineUniformName(program, GL_FRAGMENT_SHADER, i, 256, &len, name));
            std::cout << "Subroutine uniform: " << i << " - name: " << name << std::endl;

            // Number of subroutines
            glCall(glGetActiveSubroutineUniformiv(program, GL_FRAGMENT_SHADER, i, GL_NUM_COMPATIBLE_SUBROUTINES, &numCompS));

            int * s =  new int [numCompS];
            glCall(glGetActiveSubroutineUniformiv(program, GL_FRAGMENT_SHADER, i, GL_COMPATIBLE_SUBROUTINES, s));
            std::cout << "Compatible subroutines: " << std::endl;
            
            for (int j = 0; j < numCompS; ++j) {
                glCall(glGetActiveSubroutineName(program, GL_FRAGMENT_SHADER, s[j], 256, &len, name));
                std::cout << "\t" << s[j] << " - " << name << std::endl;
                this->shaders.push_back(name);
            }
            std::cout << std::endl;

            delete [] s;
        }
    }

    void PrintCurrentShader(int subroutine){
        std::cout << "Current shader subrouting: " << shaders[subroutine] << std::endl;
    }

    void render(GLuint program){ 
        GLuint index = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, shaders[current_subroutine].c_str());
        glCall(glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index));
    }


public:
    Renderer(glm::mat4 projection) {
        this->projection = projection;
    }
    
};

#endif