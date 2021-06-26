#pragma once

using namespace std;

#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

class Mesh
{
    public:

        vector<Vertex> vertices;
        vector<GLuint> indices;
        GLuint VAO;

        Mesh(const Mesh& copy) = delete;
        Mesh& operator=(const Mesh &) = delete;

        Mesh(vector<Vertex>& vertices, vector<GLuint>& indices) noexcept : vertices(std::move(vertices)), indices(std::move(indices))
        {
            this->setupMesh();
        }
        
        Mesh(Mesh&& move) noexcept : vertices(std::move(vertices)), indices(std::move(indices)), VAO(move.VAO), VBO(move.VBO), EBO(move.EBO)
        {
            move.VAO = 0;
        }

        Mesh& operator=(Mesh&& move) noexcept 
        {
            freeGPUresources();

            if (move.VAO)
            {
                vertices = std::move(move.vertices);
                indices = std::move(move.indices);
                VAO = move.VAO;
                VBO = move.VBO;
                EBO = move.EBO;

                move.VAO = 0;
            }
            else
            {
                VAO = 0;
            }
            return *this;
        }

        Mesh() noexcept
        {
            freeGPUresources();
        }
};