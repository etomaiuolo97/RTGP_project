/*
Model class - v1
- OBJ models loading using Assimp library
- the class converts data from Assimp data structure to a OpenGL-compatible data structure (Mesh class in mesh_v1.h)

N.B. 1)  
Model and Mesh classes follow RAII principles (https://en.cppreference.com/w/cpp/language/raii).
Model is a "move-only" class. A move-only class ensures that you always have a 1:1 relationship between the total number of resources being created and the total number of actual instantiations occurring.

N.B. 2) no texturing in this version of the class

N.B. 3) based on https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/model.h

authors: Davide Gadia, Michael Marchesan

Real-Time Graphics Programming - a.a. 2020/2021
Master degree in Computer Science
Universita' degli Studi di Milano
*/



#pragma once

#ifndef MODEL
#define MODEL

using namespace std;

// we use GLM data structures to convert data in the Assimp data structures in a data structures suited for VBO, VAO and EBO buffers
#include <glm/glm.hpp>

// Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// we include the Mesh class, which manages the "OpenGL side" (= creation and allocation of VBO, VAO, EBO buffers) of the loading of models
#include <utils/system/mesh.h>

/////////////////// MODEL class ///////////////////////
class Model
{
public:
    // at the end of loading, we will have a vector of Mesh class instances
    vector<Mesh> meshes;

    //////////////////////////////////////////

    // We want Model to be a move-only class. We delete copy constructor and copy assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/copy_constructor
    // https://en.cppreference.com/w/cpp/language/copy_assignment
    // https://www.geeksforgeeks.org/preventing-object-copy-in-cpp-3-different-ways/
    // Section 4.6 of the "A Tour in C++" book
    Model(const Model& model) = delete; //disallow copy
    Model& operator=(const Model& copy) = delete;
    
    // For the Model class, a default move constructor and move assignment is sufficient
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/move_constructor
    // https://en.cppreference.com/w/cpp/language/move_assignment
    // https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
    // https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
    // Section 4.6 of the "A Tour in C++" book
    Model& operator=(Model&& move) noexcept = default;
    Model(Model&& model) = default; //internally does a memberwise std::move
    
    Model(){}

    // constructor
    // to notice that Model class is not strictly following the Rules of 5 
    // https://en.cppreference.com/w/cpp/language/rule_of_three
    // because we are not writing a user-defined destructor.
    Model(const string& path)
    {
        this->loadModel(path);
    }

    //////////////////////////////////////////

    // model rendering: calls rendering methods of each instance of Mesh class in the vector
    void Draw() {
        for(GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw();
    }

private:

    void loadModel(string path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        // check for errors (see comment above)
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {// if is Not Zero
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        // we start the recursive processing of nodes in the Assimp data structure
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
                // cout << "WARNING::ASSIMP:: MODEL WITHOUT UV COORDINATES -> TANGENT AND BITANGENT ARE = 0" << endl;
            }
            // we add the vertex to the list
            vertices.push_back(vertex);
        }

        // for each face of the mesh, we retrieve the indices of its vertices , and we store them in a vector data structure
        for(GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // we return an instance of the Mesh class created using the vertices and faces data structures we have created above.
        return Mesh(vertices, indices);
    }
};

#endif