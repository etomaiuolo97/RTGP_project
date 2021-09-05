#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

#include <utils/shader.h>
#include <utils/camera.h>

// Represents a single particle and its state
struct particleProps
{
	glm::vec2 position,velocity,velocityVariation;
	glm::vec4 color;
	float lifeTime = 1.0;
	float size;
    //particleProps() : position(0.0f), velocity(0.0f), color(1.0), lifeTime(1.0f), size(0.5f) { }
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
    void Render(Camera camera);
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
		float gravityEffect;

		bool active = false;
	};

    std::vector<Particle> m_particlePool;
	uint32_t m_PoolIndex = 999;
	float GRAVITY = -50;

	GLuint m_QuadVA = 0;
	Shader m_ParticleShader = Shader("particle_model.vert","particle_model.frag");
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};

#endif

#define GLM_ENABLE_EXPERIMENTAL

particle_model::particle_model()
{
	this->m_particlePool.resize(1000);
}

void particle_model::Update(float ts)
{
	for (Particle& particle : m_particlePool)
	{
		if (!particle.active)
			continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= ts;
		particle.position += particle.velocity * (float)ts;
	}
}

void particle_model::Render(Camera camera)
{
    if (!m_QuadVA)
	{
		float vertices[] = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f
		};

		glCreateVertexArrays(1, &m_QuadVA);
		glBindVertexArray(m_QuadVA);

		GLuint quadVB, quadIB;
		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(quadVB, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    	
		m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShaderViewProj, "u_ViewProj");
		m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShaderTransform, "u_Transform");
		m_ParticleShaderColor = glGetUniformLocation(m_ParticleShaderColor, "u_Color");
	}

	//glUseProgram(m_ParticleShader.GetRendererID());
	m_ParticleShader.Use();
	glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

	for (Particle& particle : this->m_particlePool)
	{
		if (!particle.active)
			continue;

		// Fade away particles
		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = particle.color * life;

		float size = particle.size;
		
		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
		glBindVertexArray(m_QuadVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void particle_model::Emit(const particleProps& particleProps)
{
	std::cout << glGetError() << std::endl;
	Particle& particle = m_particlePool[m_PoolIndex];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = (float)rand() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocityVariation.x * GRAVITY;
	particle.velocity.y += particleProps.velocityVariation.y * GRAVITY;

	// Color
	particle.color = particleProps.color;

	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;
	particle.size = particleProps.size;

	m_PoolIndex = --m_PoolIndex % m_particlePool.size();
}
