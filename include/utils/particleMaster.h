#pragma once

#include <utils/particleRenderer.h>
#include <vector>

class particleMaster
{
private:
    std::vector<Particle> particles;
    particleRenderer renderer;

public:
    particleMaster();
    ~particleMaster();
    void init(glm::mat4 projectionMatrix){
        renderer = particleRenderer(projectionMatrix);
    }

    void update(){
        for(Particle particle: particles){
            bool stillAlive = particle.update();
            if (!stillAlive)
            {
                particles.pop_back();
            }
        }      
    }

    void renderParticles(Camera camera){
        renderer.render(particles,camera);
    }

    void cleanUp(){
        renderer.cleanUp();
    }

    void addParticle(Particle particle){
        particles.push_back(particle);
    }
};