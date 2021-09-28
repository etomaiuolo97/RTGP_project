#pragma once

#ifndef PARTICLE_TEXTURE
#define PARTICLE_TEXTURE

class ParticleTexture {
private:
    GLuint textureID;
    GLuint numRows;

public:
    ParticleTexture () {}

    ParticleTexture (GLuint textureID, GLuint numRows) {
        this->textureID = textureID;
        this->numRows = numRows;
    }

    GLuint getTextureID () {
        return this->textureID;
    }

    GLuint getNumRows () {
        return this->numRows;
    }

};

#endif
