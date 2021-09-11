#pragma once

#include <utils/particle.h>
#include <utils/particleShader.h>
#include <utils/camera.h>

class particleRenderer
{
private:
    GLfloat vertices[9] = {
        -0.5f,0.5f,-0.5f,
        -0.5f,0.5f,0.5f,
        0.5f,-0.5f};

    GLuint indices[6] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    GLuint quad;
    particleShader shader;

    void prepare(){
        shader.start();
        glCall(glBindVertexArray(this->quad));
        glCall(glEnableVertexAttribArray(0));
        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA));
        glCall(glDepthMask(false));
    }

    void finishRendering(){
        glDepthMask(true);
        glDisable(GL_BLEND);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        shader.stop();
    }

    glm::mat4 createViewMatrix(Camera camera) {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.roll), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 cameraPos = camera.position;
        glm::vec3 negCameraPos = glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
        matrix = glm::translate(matrix, negCameraPos);

        return matrix;
    }

public:
    
    ~particleRenderer();

    particleRenderer(glm::mat4 projectionMatrix){
        GLuint VBO, VAO, EBO;
        glCall(glGenVertexArrays(1, &VAO));
        glCall(glGenBuffers(1, &VBO));
        glCall(glGenBuffers(1, &EBO));
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glCall(glBindVertexArray(VAO));

        glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

        glCall(glEnableVertexAttribArray(0));
        glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0));

        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glCall(glBindVertexArray(0));

        shader.start();
        shader.loadProjectionMatrix(projectionMatrix);
        shader.stop(); 
    }

    void render(std::vector<Particle> particles, Camera camera){
        glm::mat4 viewMatrix = createViewMatrix(camera);
        prepare();        
        for(Particle particle: particles){
            updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), viewMatrix);
            glCall(glDrawArrays(GL_TRIANGLE_STRIP,0,sizeof(quad)));           
        }
        finishRendering();
    }

    void cleanUp(){
        shader.cleanUp();
    }

    void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix){
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix,position);
        // sets rotation of model matrix to transpose of rotation of view matrix in 3*3
        //modelMatrix = glm::transpose(viewMatrix);
        modelMatrix[0,0] = viewMatrix[0,0];
        modelMatrix[0,1] = viewMatrix[1,0];
        modelMatrix[0,2] = viewMatrix[2,0];
        modelMatrix[1,0] = viewMatrix[0,1];
        modelMatrix[1,1] = viewMatrix[1,1];
        modelMatrix[1,2] = viewMatrix[2,1];
        modelMatrix[2,0] = viewMatrix[0,2];
        modelMatrix[2,1] = viewMatrix[1,2];
        modelMatrix[2,2] = viewMatrix[2,2];
        
        modelMatrix = glm::rotate(modelMatrix,glm::radians(rotation),glm::vec3(0,0,1));
        modelMatrix = glm::scale(modelMatrix,glm::vec3(scale,scale,scale));
        glm::mat4 modelViewMatrix= viewMatrix*modelMatrix;
        shader.loadViewMatrix(modelViewMatrix);
    }

};