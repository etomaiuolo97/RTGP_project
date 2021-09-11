#pragma once

#include <transform.h>

class Renderable {
public:

    Transform* transform;
    Mesh* mesh;
    Shader* shader;
    GLint* texture;

    GLfloat textureTiling;

    Renderable(){
        this->transform = new Transform();
        this->textureTiling = 1.0f;
    }
};