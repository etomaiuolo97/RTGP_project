#ifndef PARTICLE_GENERATOR
#define PARTICLE_GENERATOR

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <utils/particle/particle_shader.h>
#include <utils/system/utils.h>

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;

    GLfloat life;

    Particle(): position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

class ParticleGenerator {
public:
    ParticleGenerator (glm::mat4 projection, GLuint texture, GLuint amount)
        : texture(texture), amount(amount) {
        
        this->shader.start();
        this->shader.loadSprite(0);
        this->shader.loadProjection(projection);
        this->shader.stop();

        this->init();
    }

    void update (GLfloat deltaTime, GLuint newParticle, glm::vec2 offset = glm::vec2(0.0f, 0.0f)){
        for (GLuint i = 0; i < newParticle; ++i) {
            GLint unusedParticle = this->firstUnusedParticle();
            this->respawnParticle(this->particles[unusedParticle], offset);
        }

        for (GLuint i = 0; i < this-> amount; ++i) {
            Particle &p = this->particles[i];
            p.life -= deltaTime;

            if (p.life > 0.0f) {
                p.position -= p.velocity * deltaTime;
                p.color.a -= deltaTime * 2.5f;
            }
        }
    }

    void Draw () {
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
        this->shader.start();

        for (Particle particle: this->particles) {
            if (particle.life > 0.0f) {
                this->shader.loadOffset(particle.position);
                this->shader.loadColor(particle.color);
                
                glCall(glActiveTexture(GL_TEXTURE0));
                glCall(glBindTexture(GL_TEXTURE_2D, this->texture));

                glCall(glBindVertexArray(this->VAO));
                glCall(glDrawArrays(GL_TRIANGLES, 0, 6));
                glCall(glBindVertexArray(0));
            }
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->shader.stop();
    }

    void cleanUp() {
        this->shader.cleanUp();
    }

private:
    std::vector<Particle> particles;
    GLuint amount;

    ParticleShader shader;
    GLuint texture;
    GLuint VAO;

    void init (){
        GLuint VBO;

        GLfloat particle_quad [] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        glCall(glGenVertexArrays(1, &this->VAO));
        glCall(glGenBuffers(1, &VBO));
        glCall(glBindVertexArray(this->VAO));

        // Fill mesh buffer
        glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW));

        // Set mesh attributes
        glCall(glEnableVertexAttribArray(0));
        glCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void *) 0));
        glCall(glBindVertexArray(0));

        // Create this->amount default particle instances
        for (GLuint i = 0; i < this->amount; ++i) 
            this->particles.push_back(Particle());
    }

    GLuint lastUsedParticle = 0;
    GLuint firstUnusedParticle () {
        // First search from last used particle, this will usually return almost instantly
        for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
            if (this->particles[i].life <= 0.0f) {
                lastUsedParticle = i;
                return i;
            }
        }

        // Otherwise, do a linear search
        for (GLuint i = 0; i < lastUsedParticle; ++i) {
            if (this->particles[i].life <= 0.0f){
                lastUsedParticle = i;
                return i;
            }
        }

        // All particles are taken, ovverride the first one
        lastUsedParticle = 0;
        return 0;
    }

    void respawnParticle (Particle& particle, glm::vec2 offset = glm::vec2(0.0f, 0.0f)) {
        GLfloat random = (GLfloat) (((rand() % 100) - 50) / 10.0f);
        GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);

        particle.position = glm::vec2(0.0f) + random + offset;
        particle.life = 1.0f;
        // particle.velocity = 0.1f;
    }
};

#endif
