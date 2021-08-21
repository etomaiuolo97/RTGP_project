#pragma once
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <utils/shader.h>

// Represents a single particle and its state
struct particleProps
{
	glm::vec2 position,velocity;
	glm::vec4 color;
	float lifeTime = 1.0;
	float size;
    particleProps() : position(0.0f), velocity(0.0f), color(1.0f), lifeTime(0.0f) { }
};

// particle_model acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class particle_model{
public:
    // constructor
    particle_model();
    // update all particles
    void Update(float dt);
    // render all particles
    void Render();
    // emit particles
    void Emit(const particleProps& particle_props);

private:
	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 color;

		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;
		float rotation = 0.0f;
		float size;

		bool active = false;
	};

    std::vector<Particle> m_particlePool;
	uint32_t m_PoolIndex = 999;

	GLuint m_QuadVA = 0;
	Shader m_ParticleShader;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};

#endif

