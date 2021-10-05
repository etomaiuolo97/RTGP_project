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
    GLfloat velocityVariation;

    glm::vec4 colorBegin, colorEnd;

    GLfloat sizeBegin, sizeEnd, sizeVariation;

    GLfloat life;

    glm::vec3 direction = {0.0f, 1.0f, 0.0f};
    GLfloat directionDev = 90.0f;
};

struct ParticleVertex {
    glm::vec2 position;
};

class ParticleGenerator {
public:
    ParticleGenerator (){
        this->particles.resize(10000);
    }

    void update (GLfloat deltaTime){
        for (auto& particle : this->particles) {
            if (particle.active){
                if (particle.lifeRemaining <= 0.0f) {
                    particle.active = false;
                }
                else {                    
                    // Position update
                    particle.velocity.y += gravity * deltaTime;
                    particle.position.y += particle.velocity.y * (GLfloat) deltaTime;

                    if (particle.position.y < 0.0)
                        particle.position.y = 0.0;
                    else {
                        particle.position.x += particle.velocity.x * (GLfloat) deltaTime;
                        particle.position.z += particle.velocity.z * (GLfloat) deltaTime;
                    }
                    
                    // Life update
                    particle.lifeRemaining -= deltaTime;
                } 
            }
        }
    }

    void Draw (glm::mat4& viewMatrix) {
        shader.loadViewMatrix(viewMatrix);

        for (auto& particle: this->particles) {
            if (particle.active){
                GLfloat life = particle.lifeRemaining / particle.lifeTime;

                glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);

                GLfloat size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
                
                GLfloat blend = glm::lerp(1.0f, 0.0f, life);

                glm::mat4 model = this->createModelMatrix(particle.position, particle.rotation, size, viewMatrix);
                shader.loadModelMatrix(model);

                this->waterDrop.Draw();
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

        GLfloat blend;
        glm::vec2 texOffset1;
        glm::vec2 texOffset2;

        bool active = false;
    };

    std::vector<Particle> particles;
    GLuint index = 999;

    ParticleShader shader;

    GLfloat gravity = -7.81f;

    Model waterDrop = Model ("./meshes/circle.obj");

    glm::mat4 createModelMatrix (glm::vec3 position, GLfloat rotation, GLfloat scale, glm::mat4 & viewMatrix) {
        glm::mat4 modelMatrix (1.0f);

        modelMatrix = glm::translate(modelMatrix, position);

        modelMatrix[0][0] = viewMatrix[0][0];
        modelMatrix[0][1] = viewMatrix[1][0];
        modelMatrix[0][2] = viewMatrix[2][0];
        modelMatrix[1][0] = viewMatrix[0][1];
        modelMatrix[1][1] = viewMatrix[1][1];
        modelMatrix[1][2] = viewMatrix[2][1];
        modelMatrix[2][0] = viewMatrix[0][2];
        modelMatrix[2][1] = viewMatrix[1][2];
        modelMatrix[2][2] = viewMatrix[2][2];

        modelMatrix = glm::rotate(modelMatrix, (GLfloat)glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));

        return modelMatrix;
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
