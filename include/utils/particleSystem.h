#pragma once

#include <utils/particle.h>
#include <glm/glm.hpp>
#include <mat4x4.hpp>

#define PI 3.141592653
#define RAND_FACTOR 2.0f


class particleSystem
{
private:
/*
    float pps;
	float speed;
	float gravityComplient;
	float lifeLength;

    particleTexture texture;

    void emitParticle(glm::vec3 center){
		float dirX = (float) random();
		float dirZ = (float) random();
		glm::vec3 velocity = glm::vec3(dirX, 1, dirZ);
		velocity = glm::normalize(velocity)*speed;
		//velocity = glm::scale(glm::vec3(speed));

		Particle(texture,glm::vec3(center), velocity, gravityComplient, lifeLength, 0, 1);
	}
	
*/
	// pps= particle per seconds
    float pps, averageSpeed, gravityComplient, averageLifeLength, averageScale;
    float speedError, lifeError, scaleError = 0;
    bool randomRotation = false;
    glm::vec3 direction;
    float directionDeviation = 0;

    particleTexture texture;

	void emitParticle(glm::vec3 center) {
        glm::vec3 velocity;
        if(direction.length()!=0){
            velocity = generateRandomUnitVectorWithinCone(direction, directionDeviation);
        }else{
            velocity = generateRandomUnitVector();
        }
		velocity = glm::normalize(velocity);
        //velocity.scale(generateValue(averageSpeed, speedError));
        velocity = velocity*generateValue(averageSpeed, speedError);
        float scale = generateValue(averageScale, scaleError);
        float lifeLength = generateValue(averageLifeLength, lifeError);
        new Particle(texture,glm::vec3(center), velocity, gravityComplient, lifeLength, generateRotation(), scale);
    }

	float generateValue(float average, float errorMargin) {
        float offset = (randf(0.05) - 0.5f) * (2.0f) * errorMargin;
        return average + offset;
    }

    float generateRotation() {
        if (randomRotation) {
            return randf(0.05) * (360.0f);
        } else {
            return 0;
        }
    }

    glm::vec3 generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle) {
        float cosAngle = (float) cos(angle);
        //Random random = Random();
        float theta = (float) (randf(0.05) * (2.0f) * PI);
        float z = cosAngle + (randf(0.05) * (1 - cosAngle));
        float rootOneMinusZSquared = (float) sqrt(1 - z * z);
        float x = (float) (rootOneMinusZSquared * cos(theta));
        float y = (float) (rootOneMinusZSquared * sin(theta));

        glm::vec4 direction = glm::vec4(x, y, z, 1);
        if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
            glm::vec3 rotateAxis = glm::cross(coneDirection, glm::vec3(0, 0, 1));
			rotateAxis = glm::normalize(rotateAxis);
            float rotateAngle = (float) acos(glm::dot(coneDirection, glm::vec3(0, 0, 1)));
            glm::mat4 rotationMatrix = glm::mat4();
            //rotationMatrix.rotate(-rotateAngle, rotateAxis);
            glRotatef(-rotateAngle,rotateAxis.x,rotateAxis.y,rotateAxis.z);
            //Matrix4f.transform(rotationMatrix, direction, direction);
            // TODO: transform matrix
            //glm::mat4 direction = glm::translate()*glm::rotate()* glm::scale();//rotationMatrix*direction;
            
        } else if (coneDirection.z == -1) {
            direction.z *= -1;
        }
        return glm::vec3(direction);
    }
    
    glm::vec3 generateRandomUnitVector() {
        float theta = (float) (randf(0.05) * (2.0f) * PI);
        float z = (randf(0.05) * 2) - 1;
        float rootOneMinusZSquared = (float) sqrt(1 - z * z);
        float x = (float) (rootOneMinusZSquared * cos(theta));
        float y = (float) (rootOneMinusZSquared * sin(theta));
        return glm::vec3(x, y, z);
    }

    // generate random floats between 0 and 1
    float randf(float range) {
        return (float)rand() / (float)RAND_MAX * range * RAND_FACTOR;
    }


public:
    ~particleSystem();
/*
    particleSystem(particleTexture texture,float pps, float speed, float gravityComplient, float lifeLength) {
		this->pps = pps;
		this->speed = speed;
		this->gravityComplient = gravityComplient;
		this->lifeLength = lifeLength;
        
        this->texture=texture;
	}

    void generateParticles(glm::vec3 systemCenter){
		float delta = (float) glfwGetTime();
		float particlesToCreate = pps * delta;
		int count = (int) floor(particlesToCreate);
		float partialParticle = (int)particlesToCreate % 1;
		for(int i=0;i<count;i++){
			emitParticle(systemCenter);
		}
		if(random() < partialParticle){
			emitParticle(systemCenter);
		}
	}
*/

	particleSystem(float pps, float speed, float gravityComplient, float lifeLength, float scale) {
        this->pps = pps;
        this->averageSpeed = speed;
        this->gravityComplient = gravityComplient;
        this->averageLifeLength = lifeLength;
        this->averageScale = scale;
    }

	 void setDirection(glm::vec3 direction, float deviation) {
        direction = glm::vec3(direction);
        directionDeviation = (float) (deviation * PI);
    }

    void randomizeRotation() {
        randomRotation = true;
    }
    
    // error: A number between 0 and 1, where 0 means no error margin.
    void setSpeedError(float error) {
        speedError = error * averageSpeed;
    }

    void setLifeError(float error) {
        lifeError = error * averageLifeLength;
    }

    void setScaleError(float error) {
        scaleError = error * averageScale;
    }

	void generateParticles(glm::vec3 systemCenter) {
        float delta = (float)glfwGetTime();
        float particlesToCreate = pps * delta;
        int count = (int) floor(particlesToCreate);
        float partialParticle = (int)particlesToCreate % 1;
        for (int i = 0; i < count; i++) {
            emitParticle(systemCenter);
        }
        if (random() < partialParticle) {
            emitParticle(systemCenter);
        }
    }
	
};
