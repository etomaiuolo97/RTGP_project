#pragma once

#ifndef GUI_TEXTURE
#define GUI_TEXTURE

#include <glad/glad.h>
#include <glm/glm.hpp>

class GuiTexture {
private:
    GLint texture;
    glm::vec2 position;
    glm::vec2 scale;

public:
    GuiTexture () {};
    
    GuiTexture (GLint texture, glm::vec2 position, glm::vec2 scale){
        this->texture = texture;
        this->position = position;
        this->scale = scale;
    }

    GLint getTexture(){
        return this->texture;
    }

    glm::vec2 getPosition(){
        return this->position;
    }

    glm::vec2 getScale(){
        return this->scale;
    }

    void setScale (glm::vec2 scale){
        this->scale = scale;
    }

};

#endif