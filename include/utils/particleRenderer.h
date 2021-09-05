#pragma once
#include <glad/glad.h>
#include <utils/particleShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utils/camera.h>
#include <utils/particle.h>
#include <list>

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

    void prepare(){
        shader.start();
        glBindVertexArray(this->quad);
        glEnableVertexAttribArray(0);
        GL_BLEND;
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA);
    }

    void finishRendering(){
        glDepthMask(true);
        glDisable(GL_BLEND);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        shader.stop();
    }

public:
    GLuint quad;
    particleShader shader;
    
    ~particleRenderer();

    particleRenderer(glm::mat4 projectionMatrix){
        GLuint VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0);

        //quad = loadtoVAO(vertices,2);
        // TODO: problems with type of shader: GLuint vs particleShader class
        shader = glCreateShader(GL_VERTEX_SHADER);
        shader.start();
        shader.loadProjectionMatrix(projectionMatrix);
        shader.stop(); 
    }

    void render(std::vector<Particle> particles, Camera camera){
        // createViewMatrix from utils.h
        glm::mat4 viewMatrix = createViewMatrix(camera);
        prepare();        
        for(Particle particle: particles){
            updateModelViewMatrix(particle.getPosition(),particle.getRotation(),
                particle.getScale(),viewMatrix);
            glDrawArrays(GL_TRIANGLE_STRIP,0,sizeof(quad));           
        }
        finishRendering();
    }

    void cleanUp(){
        shader.cleanUp();
    }

    void updateModelViewMatrix(glm::vec3 position, float rotation, float scale,
    glm::mat4 viewMatrix){
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix,position);
        // sets rotation of model matrix to transpose of rotation of view matrix in 3*3
        modelMatrix = glm::transpose(viewMatrix);
        
        // modelMatrix.m00=viewMatrix.m00;
        // modelMatrix.m22=viewMatrix.m22;
        
        modelMatrix = glm::rotate(modelMatrix,glm::radians(rotation),glm::vec3(0,0,1));
        modelMatrix = glm::scale(modelMatrix,glm::vec3(scale,scale,scale));
        glm::mat4 modelViewMatrix= viewMatrix*modelMatrix;
        shader.loadModelViewMatrix(modelViewMatrix);
    }

};