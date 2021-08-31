#ifdef _WIN32
    #define __USE_MINGW_ANSI_STDIO 0
#endif

#include <iostream>
#include <string>

#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#include <glad/glad.h>
#include <glfw/glfw3.h>

#ifdef _WINDOWS_
    #error windows.h was included!
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// #include <utils/shader.h>
#include <utils/model.h>
#include <utils/my_camera.h>
#include <utils/utils.h>
#include <utils/display.h>
#include <utils/illumination_shader.h>
#include <utils/background_shader.h>


// Setup of Shader Programs for the shader used in the application
void SetupShaders(int program);

// Print on console the name of the current shader
void PrintCurrentShader(int shader);

// Index of the currente shader (= 0  in the beginning)
GLuint current_subroutine = 0;

// Vector for all the Shader Programs used and swapped in the application
vector<std::string> shaders;

// Uniforms to pass to shaders
GLuint textureCube;

glm::vec3 lightPos[] = {glm::vec3(0.0f, 0.0f, 10.0f)};

GLfloat Eta = 1.0f/1.52f;
GLfloat mFresnelPower = 5.0f;

GLfloat diffuseColor [] = {1.0f, 1.0f, 1.0f};
GLfloat specularColor [] = {1.0f, 1.0f, 1.0f};
GLfloat ambientColor [] = {0.1f, 0.1f, 0.1f};

GLfloat Kd = 0.5f;
GLfloat Ks = 0.4f;
GLfloat Ka = 0.1f;

GLfloat shininess = 25.0f;
GLfloat alpha = 0.2f;
GLfloat F0 = 0.9f;

int main () {
    std::cout << "Starting GLFW context" << std::endl;

    GLFWwindow* window = createDisplay();

    // // Enable Z test
    glEnable(GL_DEPTH_TEST);

    // The clear color for the frame buffer
    glClearColor(0.26f, 0.46f, 0.98f, 1.0f);

    IlluminationShader illumination_shader;

    SetupShaders(illumination_shader.program);
    PrintCurrentShader(current_subroutine);

    // Shader reflection_shader("src/reflection.vert", "src/reflection.frag");
    BackgroundShader background_shader;
    textureCube = LoadTextureCube("textures/skybox/");

    Model fountainModel("meshes/ball_fountain.obj");
    Model bgModel("meshes/cube.obj");
    
    // Projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 10000.0f);
    
    // View matrix
    glm::mat4 view = glm::mat4(1.0f);

    // Model and Normal transformation matrices for the objects in the scene
    glm::mat4 fountainModMatrix = glm::mat4(1.0f);
    glm::mat3 fountainNorMatrix = glm::mat3(1.0f);
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        prepareDisplay(wireframe);
        
        // Apply FPS camera movements
        apply_camera_movements();
        view = camera.GetViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        illumination_shader.start();

        GLuint index = glGetSubroutineIndex(illumination_shader.program, GL_FRAGMENT_SHADER, shaders[current_subroutine].c_str());
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        illumination_shader.loadAmbientColor(ambientColor);
        illumination_shader.loadSpecularColor(specularColor);
        illumination_shader.loadShine(shininess);
        illumination_shader.loadAlpha(alpha);
        illumination_shader.loadF0(F0);

        illumination_shader.loadKa(0.0f);
        illumination_shader.loadKd(0.6f);
        illumination_shader.loadKs(0.0f);

        illumination_shader.loadProjectionMatrix(projection);
        illumination_shader.loadViewMatrix(view);
        illumination_shader.loadLights(lightPos);

        GLfloat diff [] = {0.5f, 0.5f, 0.5f};
        illumination_shader.loadDiffuseColor(diff);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);

        fountainModMatrix = glm::mat4(1.0f);
        fountainNorMatrix = glm::mat3(1.0f);

        fountainModMatrix = glm::translate(fountainModMatrix, glm::vec3(0.0f, -3.0f, -5.0f));
        fountainModMatrix = glm::rotate(fountainModMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        fountainModMatrix = glm::scale(fountainModMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
        fountainNorMatrix = glm::inverseTranspose(glm::mat3(view*fountainModMatrix));

        illumination_shader.loadModelMatrix(fountainModMatrix);
        illumination_shader.loadNormalMatrix(fountainNorMatrix);
        fountainModel.Draw();

        glDepthFunc(GL_LEQUAL);

        background_shader.start();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);

        background_shader.loadProjectionMatrix(projection);
        
        view = glm::mat4(glm::mat3(view));
        background_shader.loadViewMatrix(view);
        
        background_shader.loadtCube(0);

        bgModel.Draw();
        background_shader.stop();

        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
    }

    illumination_shader.cleanUp();
    // reflection_shader.Delete();
    background_shader.cleanUp();
    glfwTerminate();
    return 0;
}

void SetupShaders(int program) {
    int maxSub,maxSubU,countActiveSU;
    GLchar name[256];
    int len, numCompS;

    // Subroutines parameters of the system
    glGetIntegerv(GL_MAX_SUBROUTINES, &maxSub);
    glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &maxSubU);
    std::cout << "Max Subroutines:" << maxSub << " - Max Subroutine Uniforms:" << maxSubU << std::endl;

    // Subroutine uniforms 
    glGetProgramStageiv(program, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &countActiveSU);

    for (int i = 0; i < countActiveSU; i++) {

        // Name of subroutine uniform
        glGetActiveSubroutineUniformName(program, GL_FRAGMENT_SHADER, i, 256, &len, name);
        std::cout << "Subroutine uniform: " << i << " - name: " << name << std::endl;

        // Number of subroutines
        glGetActiveSubroutineUniformiv(program, GL_FRAGMENT_SHADER, i, GL_NUM_COMPATIBLE_SUBROUTINES, &numCompS);

        int * s =  new int [numCompS];
        glGetActiveSubroutineUniformiv(program, GL_FRAGMENT_SHADER, i, GL_COMPATIBLE_SUBROUTINES, s);
        std::cout << "Compatible subroutines: " << std::endl;
        
        for (int j = 0; j < numCompS; ++j) {
            glGetActiveSubroutineName(program, GL_FRAGMENT_SHADER, s[j], 256, &len, name);
            std::cout << "\t" << s[j] << " - " << name << std::endl;
            shaders.push_back(name);
        }
        std::cout << std::endl;

        delete [] s;
    }
}

void PrintCurrentShader(int subroutine){
    std::cout << "Current shader subrouting: " << shaders[subroutine] << std::endl;
}





