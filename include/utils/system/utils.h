// In this class the debug part is based on https://www.youtube.com/watch?v=FBbPWSOQ0-w

#ifndef UTILS
#define UTILS

#define STB_IMAGE_IMPLEMENTATION

#ifdef _WIN32
    #define ASSERT(x) if (!x) __debugbreak();
    #define PATH "./"
#else
    #define ASSERT(x) if (!x) __builtin_debugtrap();
    #define PATH "../"
#endif

#include <stb_image/stb_image.h>

#include <string>
#include <vector>

#include <glad/glad.h>

GLuint textureCube;

vector <GLuint> textures;

typedef struct {
    glm::vec3 position;
    float water_height;
    float angle;
    float life;
} particle_prop_t;

// fountainIndex: keep trace of the fountain is actually rendered 
int fountainIndex;
int numFountains = 0;
// cubeTexIndex: keep trace of the cubemap is actually rendered 
int cubeTexIndex;
int numCubeTextures = 0;

bool isNight = false;

// Discard all the errors found before
void clearErrors() {
    while(glGetError() != GL_NO_ERROR);
}

// Print the line error
bool logCall (const char* function, const char* file, int line) {
    while (GLenum error = glGetError()){
        std::cout << "[OpenGL_Error] (" << error << "): " << function <<
            "  " << file << ":" << line <<  std::endl;
        return false;
    }
    return true;
}

// Print the error
void checkErrors (){
    while (GLenum error = glGetError()){
        std::cout << "[OpenGL_Error] (" << error << std::endl;
    }
}



#define glCall(x) clearErrors(); x; ASSERT(logCall(#x, __FILE__, __LINE__));

// Function to load the texture for a side of the cubemap
void LoadTextureCubeSide (string path, string side_img, GLuint side_name) {
    int w, h;
    unsigned char * image;
    string fullname = path + side_img;

    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;

    glCall(glTexImage2D(side_name, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
    stbi_image_free(image);
}

// Function to load the texture for the cubemap
GLint LoadTextureCube (string path) {
    GLuint textureImage;

    glCall(glGenTextures(1, &textureImage));
    glCall(glActiveTexture(GL_TEXTURE0));
    glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureImage));

    path = PATH + path;

    LoadTextureCubeSide(path, std::string("posx.jpg"), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadTextureCubeSide(path, std::string("negx.jpg"), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadTextureCubeSide(path, std::string("posy.jpg"), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadTextureCubeSide(path, std::string("negy.jpg"), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadTextureCubeSide(path, std::string("posz.jpg"), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadTextureCubeSide(path, std::string("negz.jpg"), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

    return textureImage;
}

GLint LoadTexture(const char* path, bool repeat = false, bool rgba = false) {
    GLuint textureImage;
    int w, h, channels;
    unsigned char* image;

    std::string filepath = PATH;
    filepath.append(path);
    
    if (rgba)
        image = stbi_load(filepath.c_str(), &w, &h, &channels, STBI_rgb_alpha);
    else
        image = stbi_load(filepath.c_str(), &w, &h, &channels, STBI_rgb);

    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;

    glCall(glGenTextures(1, &textureImage));
    glCall(glBindTexture(GL_TEXTURE_2D, textureImage));

    // 3 channels = RGB ; 4 channel = RGBA
    if (channels==3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if (channels==4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glCall(glGenerateMipmap(GL_TEXTURE_2D));

    // Set how to consider UVs outside [0,1] range
    if (repeat) {
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    else {
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }
    

    // Set the filtering for minification and magnification
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Free the memory once we have created an OpenGL texture
    stbi_image_free(image);

    // Set the binding to 0 once we have finished
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
    return textureImage;
}

#endif