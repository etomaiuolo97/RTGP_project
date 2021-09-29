#pragma once

#ifndef PARTICLE_TEXTURE
#define PARTICLE_TEXTURE

class ParticleTexture {
private:
    GLint textureID;
    GLuint numRows;

public:
    ParticleTexture () {}

    ParticleTexture (GLint textureID, GLuint numRows) {
        this->textureID = textureID;
        this->numRows = numRows;
    }

    GLint getTextureID () {
        return this->textureID;
    }

    GLuint getNumRows () {
        return this->numRows;
    }

};

#endif
