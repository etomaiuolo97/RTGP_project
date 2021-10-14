#pragma once

#ifndef MODEL
#define MODEL

using namespace std;

// GLM data structures to convert data in the Assimp data structures in a data structures suited for VBO, VAO and EBO buffers
#include <glm/glm.hpp>

// Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Mesh class that manages the "OpenGL side" (= creation and allocation of VBO, VAO, EBO buffers) of the loading of models
#include <utils/system/mesh.h>

class Model {
public:
    // At the end of loading, we will have a vector of Mesh class instances
    vector<Mesh> meshes;

    // We want Model to be a move-only class. We delete copy constructor and copy assignment
    Model(const Model& model) = delete;             // Disallow copy
    Model& operator=(const Model& copy) = delete;
    
    // For the Model class, a default move constructor and move assignment is sufficient
    Model& operator=(Model&& move) noexcept = default;
    Model(Model&& model) = default;                 // Internally does a memberwise std::move
    
    Model(){}

    // Constructor
    Model(const string& path) {
        this->loadModel(path);
    }

    // Model rendering: calls rendering methods of each instance of Mesh class in the vector
    void Draw() {
        for(GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw();
    }

private:

    void loadModel(string path) {
        Assimp::Importer importer;

        path = PATH + path;

        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        // Check for errors (see comment above)
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {// if is Not Zero
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        // Start the recursive processing of nodes in the Assimp data structure
        this->processNode(scene->mRootNode, scene);
    }

    // Recursive processing of nodes of Assimp data structure
    void processNode(aiNode* node, const aiScene* scene) {
        for(GLuint i = 0; i < node->mNumMeshes; i++){
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.emplace_back(processMesh(mesh));
        }
        for(GLuint i = 0; i < node->mNumChildren; i++) {
            this->processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh) {
        vector<Vertex> vertices;
        vector<GLuint> indices;

        for(GLuint i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            glm::vec3 vector;
            
            // vertices coordinates
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            // Texture Coordinates
            if(mesh->mTextureCoords[0]) {
                glm::vec2 vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                // Tangents
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                // Bitangents
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else{
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            // Add the vertex to the list
            vertices.push_back(vertex);
        }

        // for each face of the mesh, we retrieve the indices of its vertices , and we store them in a vector data structure
        for(GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Return an instance of the Mesh class created using the vertices and faces data structures we have created above.
        return Mesh(vertices, indices);
    }
};

#endif
