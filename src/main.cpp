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
#include <utils/camera.h>
#include <utils/utils.h>
#include <utils/display.h>
#include <utils/illumination_shader.h>
#include <utils/background_shader.h>
#include <utils/particleMaster.h>
#include <utils/particle.h>
#include <utils/particleSystem.h>

// Setup of Shader Programs for the shader used in the application
void SetupShaders(int program);

// Print on console the name of the current shader
void PrintCurrentShader(int shader);

// Index of the currente shader (= 0  in the beginning)
GLuint current_subroutine = 0;

// Vector for all the Shader Programs used and swapped in the application
vector<std::string> shaders;

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
    textures.push_back(LoadTexture("textures/terrain.png"));
    
    // Projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 10000.0f);
    
    // View matrix
    glm::mat4 view = glm::mat4(1.0f);

    // Model and Normal transformation matrices for the objects in the scene
    glm::mat4 fountainModMatrix = glm::mat4(1.0f);
    glm::mat3 fountainNorMatrix = glm::mat3(1.0f);

    // init particle
    particleMaster particleMaster;
    particleMaster.init(projection);
    particleSystem system= particleSystem(50,25,0.3f,4);
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        prepareDisplay(wireframe);
        
        // Apply FPS camera movements
        apply_camera_movements();
        view = camera.GetViewMatrix();

        // generate particle and give the position of center of fountain
        system.generateParticles(glm::vec3(0.0f,0.0f,0.0f));
        //if(GLFW_KEY_Y){
          //  Particle(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,30.0f,0.0f),1.0f,4.0f,0.0f,1.0f);
        //}
        particleMaster.update();

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
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        
        illumination_shader.loadRepeat(1.0f);
        illumination_shader.loadTex(0);

        GLfloat diff [] = {0.5f, 0.5f, 0.5f};
        illumination_shader.loadDiffuseColor(diff);

        
        fountainModMatrix = createTransformationMatrix(glm::vec3(0.0f, -3.0f, -5.0f), -90.0f, 0.0f, 0.0f, 0.8f);
        fountainNorMatrix = createNormalMatrix(view, fountainModMatrix);

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

        // render particles
        particleMaster.renderParticles(camera);

        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
    }

    illumination_shader.cleanUp();
    // reflection_shader.Delete();
    background_shader.cleanUp();
    // delete particles
    particleMaster.cleanUp();
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





