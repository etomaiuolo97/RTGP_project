#pragma once

#ifndef MESH
#define MESH

using namespace std;

// Std. Includes
#include <vector>

#include <utils/utils.h>

// data structure for vertices
struct Vertex {
    // vertex coordinates
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // Texture coordinates
    glm::vec2 TexCoords;
    // Tangent
    glm::vec3 Tangent;
    // Bitangent
    glm::vec3 Bitangent;
};

/////////////////// MESH class ///////////////////////
class Mesh {
public:
    // data structures for vertices, and indices of vertices (for faces)
    vector<Vertex> vertices;
    vector<GLuint> indices;
    // VAO
    GLuint VAO;

    // We want Mesh to be a move-only class. We delete copy constructor and copy assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/copy_constructor
    // https://en.cppreference.com/w/cpp/language/copy_assignment
    // https://www.geeksforgeeks.org/preventing-object-copy-in-cpp-3-different-ways/
    // Section 4.6 of the "A Tour in C++" book
    Mesh(const Mesh& copy) = delete; //disallow copy
    Mesh& operator=(const Mesh &) = delete;

    // Constructor
    // We use initializer list and std::move in order to avoid a copy of the arguments
    // This constructor empties the source vectors (vertices and indices)
    Mesh(vector<Vertex>& vertices, vector<GLuint>& indices) noexcept
        : vertices(std::move(vertices)), indices(std::move(indices))
    {
        this->setupMesh();
    }

    // We implement a user-defined move constructor and move assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/move_constructor
    // https://en.cppreference.com/w/cpp/language/move_assignment
    // https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
    // https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
    // Section 4.6 of the "A Tour in C++" book

    // Move constructor
    // The source object of a move constructor is not expected to be valid after the move.
    // In our case it will no longer imply ownership of the GPU resources and its vectors will be empty.
    Mesh(Mesh&& move) noexcept
        // Calls move for both vectors, which internally consists of a simple pointer swap between the new instance and the source one.
        : vertices(std::move(move.vertices)), indices(std::move(move.indices)),
        VAO(move.VAO), VBO(move.VBO), EBO(move.EBO)
    {
        move.VAO = 0; // We *could* set VBO and EBO to 0 too,
        // but since we bring all the 3 values around we can use just one of them to check ownership of the 3 resources.
    }

    // Move assignment
    Mesh& operator=(Mesh&& move) noexcept
    {
        // calls the function which will delete (if needed) the GPU resources for this instance
        freeGPUresources();

        if (move.VAO) // source instance has GPU resources
        {
            vertices = std::move(move.vertices);
            indices = std::move(move.indices);
            VAO = move.VAO;
            VBO = move.VBO;
            EBO = move.EBO;

            move.VAO = 0;
        }
        else // source instance was already invalid
        {
            VAO = 0;
        }
        return *this;
    }

    Mesh(){};

    // destructor
    ~Mesh() noexcept
    {
        // calls the function which will delete (if needed) the GPU resources
        freeGPUresources();
    }

    //////////////////////////////////////////

    // rendering of mesh
    void Draw()
    {
        // VAO is made "active"
        glCall(glBindVertexArray(this->VAO));
        // rendering of data in the VAO
        glCall(glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0));
        // VAO is "detached"
        glCall(glBindVertexArray(0));
    }

private:

    // VBO and EBO
    GLuint VBO, EBO;

    //////////////////////////////////////////
    // buffer objects\arrays are initialized
    // a brief description of their role and how they are binded can be found at:
    // https://learnopengl.com/#!Getting-started/Hello-Triangle
    // (in different parts of the page), or here:
    // http://www.informit.com/articles/article.aspx?p=1377833&seqNum=8
    void setupMesh()
    {
        // we create the buffers
        glCall(glGenVertexArrays(1, &this->VAO));
        glCall(glGenBuffers(1, &this->VBO));
        glCall(glGenBuffers(1, &this->EBO));

        // VAO is made "active"
        glCall(glBindVertexArray(this->VAO));
        // we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glCall(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));
        glCall(glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW));
        // we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO));
        glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW));

        // we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
        // vertex positions
        // these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
        glCall(glEnableVertexAttribArray(0));
        glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));
        // Normals
        glCall(glEnableVertexAttribArray(1));
        glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal)));
        // Texture Coordinates
        glCall(glEnableVertexAttribArray(2));
        glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords)));
        // Tangent
        glCall(glEnableVertexAttribArray(3));
        glCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent)));
        // Bitangent
        glCall(glEnableVertexAttribArray(4));
        glCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent)));

        glCall(glBindVertexArray(0));
    }

    //////////////////////////////////////////

    void freeGPUresources()
    {
        // If VAO is 0, this instance of Mesh has been through a move, and no longer owns GPU resources,
        // so there's no need for deleting.
        if (VAO)
        {
            glCall(glDeleteVertexArrays(1, &this->VAO));
            glCall(glDeleteBuffers(1, &this->VBO));
            glCall(glDeleteBuffers(1, &this->EBO));
        }
    }
};

#endif