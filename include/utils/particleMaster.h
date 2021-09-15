#pragma once

#ifndef PARTICLE_MASTER
#define PARTICLE_MASTER

using namespace std;

#include <utils/particle.h>
#include <utils/particleRenderer.h>
#include <vector>
//#include <utils/insertionSort.h>

// Gestore di particelle con un vettore di particelle
class particleMaster
{
private:
    vector<Particle> particles;
    //std::map<particleTexture,vector<Particle> > particles;
    particleRenderer renderer;

public:
    particleMaster();
    ~particleMaster();

    void init(glm::mat4 projectionMatrix){
        renderer = particleRenderer(projectionMatrix);
    }

    // se non è più vivo lo toglie dall'array
    void update(Camera camera){
        // iterate a list of particles
/*        std::iterator<Entry<particleTexture,vector<Particle> > > mapIterator = particles.entrySet().iterator();
        while (mapIterator.hasNext())
        {
            vector<Particle> list = mapIterator.next().getValue();
            // iterate all particle for each list
            for(vector<Particle> particle: particles){
                bool stillAlive = particle.update(camera);
                if (!stillAlive)
                {
                    particles.pop_back();
                    if(list.isEmpty()){
                        mapIterator.remove();
                    }
                }
            }
            insertionSort.sortHighToLow(list);
        }
  */      

 /*
        for(Particle particle: this->particles){
            bool stillAlive = particle.update(camera);
            if (!stillAlive)
            {
                particles.pop_back();
            }
        }   

        */ 

        for (int i = 0; i < particles.size(); i++){
            bool stillAlive = particles[i].update(camera);
            if (!stillAlive){
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
        /*
        std::vector<Particle> list = particles.get(particle.getTexture());
        if(list.size()==0){
            list.push_back(particle.getTexture());
        }
        list.push_back(particle);
        */
    }

};

#endif