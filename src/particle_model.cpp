#include "particle_model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

particle_model::particle_model()
{
	m_particlePool.resize(1000);
}

void particle_model::Update(float ts)
{
	for (auto& particle : m_particlePool)
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

void particle_model::Render()
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
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		m_ParticleShader = Shader("particle_model.vert","particle_model.frag");
    	
		m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShaderViewProj, "u_ViewProj");
		m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShaderTransform, "u_Transform");
		m_ParticleShaderColor = glGetUniformLocation(m_ParticleShaderColor, "u_Color");
	}

	m_ParticleShader.Use();

	for (auto& particle : m_particlePool)
	{
		if (!particle.active)
			continue;

		// Fade away particles
		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = color * life;

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
	Particle& particle = m_particlePool[m_PoolIndex];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = (float)random() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = particleProps.velocity;

	// Color
	particle.color = particleProps.color;

	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;
	particle.size = particleProps.size;

	m_PoolIndex = --m_PoolIndex % m_particlePool.size();
}