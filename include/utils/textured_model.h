#pragma once

using namespace std;

#include <glad/glad.h>
#include <glm/glm.hpp>

class TexturedModel {
public:
    Mesh model;
    ModelTexture texture;

    TexturedModel() {};

    TexturedModel (Mesh model, ModelTexture texture) {
        this->model = model;
        this->texture = texture;
    }
};
