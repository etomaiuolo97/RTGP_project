#ifndef PARTICLE_GENERATOR
#define PARTICLE_GENERATOR

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define TIME_STEP 0.01

#include <glm/gtx/compatibility.hpp>

#include <utils/particle/particle_shader.h>
#include <utils/system/utils.h>
#include <utils/system/random.h>

struct ParticleProps {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 colorBegin, colorEnd;
    GLfloat velocityVariation;
    GLfloat sizeBegin, sizeEnd, sizeVariation;
    GLfloat life = 2.0f;
    GLfloat speedError = 0.4f;
    GLfloat lifeError = 0.1f;
    GLfloat scaleError = 0.8f;
    GLboolean randomRotation = GL_FALSE;
    glm::vec3 direction = {0.0f, 1.0f, 0.0f};
    GLfloat directionDev = 80.0f;
};

struct ParticleVertex {
    glm::vec3 position;
};

class ParticleGenerator {
public:
    ParticleGenerator (){
        this->particles.resize(10000);
    }

    void update (GLfloat deltaTime){
        for (auto& particle : this->particles) {
            if (particle.active){
                // std::cout << particle.lifeRemaining << std::endl;
                if (particle.lifeRemaining <= 0.0f) {
                    particle.active = false;
                }
                else {
                    particle.lifeRemaining -= deltaTime;
                    particle.velocity.y += gravity * deltaTime;
                    particle.position.x += particle.velocity.x * (GLfloat) deltaTime;
                    particle.position.y += particle.velocity.y * (GLfloat) deltaTime;
                    particle.position.z += particle.velocity.z * (GLfloat) deltaTime;
                } 
            }
            // particle.rotation += 0.01 * deltaTime;
        }
    }

    void Draw () {
        if (!this->VAO) {
        
            vector<ParticleVertex> vertices;
            
            ParticleVertex temp; 
            temp.position = glm::vec3(-0.5f, -0.5f, 0.0f);
            vertices.push_back(temp);
            temp.position = glm::vec3(0.5f, -0.5f, 0.0f);
            vertices.push_back(temp);
            temp.position = glm::vec3(0.5f,  0.5f, 0.0f);
            vertices.push_back(temp);
            temp.position = glm::vec3(-0.5f,  0.5f, 0.0f);
            vertices.push_back(temp);

            vector<GLuint> indices = {0, 1, 2, 2, 3, 0};

            GLuint VBO, EBO;
            glCall(glGenVertexArrays(1, &this->VAO));
            glCall(glGenBuffers(1, &VBO));
            glCall(glGenBuffers(1, &EBO));

            glCall(glBindVertexArray(this->VAO));

            glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
            glCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ParticleVertex), &vertices[0], GL_STATIC_DRAW));

            glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
            glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW));

            glCall(glEnableVertexAttribArray(0));
            glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*) 0));
            
            glCall(glBindVertexArray(0));
        }

        for (auto& particle: this->particles) {
            if (particle.active){
                GLfloat life = particle.lifeRemaining / particle.lifeTime;

                glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);

                GLfloat size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(particle.position.x, particle.position.y, 1.0f))
                            * glm::rotate(glm::mat4(1.0f), particle.rotation, glm::vec3(0.0f, 0.0f, 1.0f))
                            * glm::scale(glm::mat4(1.0f), glm::vec3(size, size, 1.0f));
                
                glm::mat4 matrix = glm::mat4(1.0f);
                matrix = glm::translate(matrix, glm::vec3(particle.position));
                matrix = glm::rotate(matrix, particle.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
                matrix = glm::scale(matrix, glm::vec3(size, size, size));

                shader.loadTransform(matrix);
                shader.loadColor(color);

                glCall(glBindVertexArray(this->VAO));
                glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
                glCall(glBindVertexArray(0));
                
            }
        }

    }

    void emit(const ParticleProps& particleProps) {        
        Particle& particle = this->particles[index];
        particle.active = true;
        particle.position = particleProps.position;
        particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

        if (particleProps.direction != glm::vec3(0.0f, 0.0f, 1.0f)) {
            particle.velocity = generateRandomUnitVectorWithinCone(particleProps.direction, particleProps.directionDev);
        }
        else {
            particle.velocity = generateRandomUnitVector();
        }
        particle.velocity = glm::normalize(particle.velocity);
        particle.velocity *= particleProps.velocityVariation;

        particle.colorBegin = particleProps.colorBegin;
        particle.colorEnd = particleProps.colorEnd;

        particle.lifeTime = particleProps.life;
        particle.lifeRemaining = particleProps.life;
        particle.sizeBegin = particleProps.sizeBegin - particleProps.sizeVariation * (Random::Float() - 0.5f);
        particle.sizeEnd = particleProps.sizeEnd;

        index = --index % this->particles.size();
    }

    void cleanUp(){
        this->shader.cleanUp();
    }

    ParticleShader& getShader() {
        return this->shader;
    }

private:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 colorBegin, colorEnd;

        GLfloat rotation = 0.0f;
        GLfloat sizeBegin, sizeEnd;

        GLfloat lifeTime = 1.0f;
        GLfloat lifeRemaining = 0.0f;

        bool active = false;
    };

    std::vector<Particle> particles;
    GLuint index = 999;

    ParticleShader shader;
    GLuint VAO = 0;
    GLfloat gravity = -9.81f;

    GLfloat generateValue (GLfloat average, GLfloat error) {
        GLfloat offset = (Random::Float() - 0.5f) * 2.0f * error;
        return average + offset;
    }

    GLfloat generateRotation () {
        return Random::Float() * 360.0f;
    }

    glm::vec3 generateRandomUnitVectorWithinCone (glm::vec3 coneDirection, GLfloat angle) {
        GLfloat cosAngle = (GLfloat) glm::cos(glm::radians(angle));
        GLfloat theta = Random::Float() * 2.0f * glm::pi<float>();
        GLfloat z = cosAngle + Random::Float() * (1 - cosAngle);
        GLfloat rootMinusZSquared = (GLfloat) glm::sqrt(1 - z * z);
        GLfloat x = rootMinusZSquared * glm::cos(theta);
        GLfloat y = rootMinusZSquared * glm::sin(theta);

        glm::vec4 direction = glm::vec4(x, y, z, 1.0f);
        if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
            glm::vec3 rotateAxis = glm::cross(coneDirection, glm::vec3(0.0f, 0.0f, 1.0f));
            rotateAxis = glm::normalize(rotateAxis);

            GLfloat rotateAngle = (GLfloat) glm::acos(glm::dot(coneDirection, glm::vec3(0.0f, 0.0f, 1.0f)));
            glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0), -rotateAngle, rotateAxis);
            direction = rotationMat * direction;
        }
        else if (coneDirection.z == -1) {
            direction.z *= -1;
        }

        return glm::vec3(direction);
    }

    glm::vec3 generateRandomUnitVector () {
        GLfloat theta = (GLfloat) (Random::Float() * 2.0f * glm::pi<float>());
        GLfloat z = (Random::Float() * 2.0f - 1.0f);
        GLfloat rootMinusZSquared = (GLfloat) glm::sqrt(1 - z * z);
        GLfloat x = (GLfloat) (rootMinusZSquared * glm::cos(theta));
        GLfloat y = (GLfloat) (rootMinusZSquared * glm::sin(theta));

        return glm::vec3 (x, y, z);
    }

};

#endif
