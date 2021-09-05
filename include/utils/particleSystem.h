#pragma once

#include <utils/particle.h>
#include <glm/glm.hpp>

#define PI 3.141592653

class particleSystem
{
private:

    float pps;
	float speed;
	float gravityComplient;
	float lifeLength;

    void emitParticle(glm::vec3 center){
		float dirX = (float) random();
		float dirZ = (float) random();
		glm::vec3 velocity = glm::vec3(dirX, 1, dirZ);
		velocity = glm::normalize(velocity)*speed;
		//velocity = glm::scale(glm::vec3(speed));

		Particle(glm::vec3(center), velocity, gravityComplient, lifeLength, 0, 1);
	}

public:
    ~particleSystem();

    particleSystem(float pps, float speed, float gravityComplient, float lifeLength) {
		this->pps = pps;
		this->speed = speed;
		this->gravityComplient = gravityComplient;
		this->lifeLength = lifeLength;
	}

    void generateParticles(glm::vec3 systemCenter){
		float delta = (float) glfwGetTime();
		float particlesToCreate = pps * delta;
		int count = (int) floor(particlesToCreate);
		//TODO: problems with type
		float partialParticle = particlesToCreate % 1;
		for(int i=0;i<count;i++){
			emitParticle(systemCenter);
		}
		if(random() < partialParticle){
			emitParticle(systemCenter);
		}
	}

	
};
