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
        
        std::vector<Particle>::iterator iter;
        // TODO: iterator and vector
        for (iter = particles.begin(); iter < particles.end(); iter++)
        {
            Particle p = particles.iterator();
            bool stillAlive = p.update();
            if (!stillAlive)
            {
                iter.remove();
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