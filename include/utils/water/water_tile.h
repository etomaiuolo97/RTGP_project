#pragma once

#ifndef WATER_TILE
#define WATER_TILE

#include <glad/glad.h>

class WaterTile {
private:
    GLfloat height;
    GLfloat x, z;

public:
    const GLfloat TILE_SIZE = 2.8;

    WaterTile (GLfloat centerX, GLfloat centerZ, GLfloat height) {
        this->x = centerX;
        this->z = centerZ;
        this->height = height;
    }

    GLfloat getHeight (){
        return this->height;
    }

    GLfloat getX(){
        return this->x;
    }

    GLfloat getZ(){
        return this->z;
    }

};

#endif