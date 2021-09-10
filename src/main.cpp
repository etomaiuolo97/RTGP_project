#ifdef _WIN32
    #define __USE_MINGW_ANSI_STDIO 0
#endif

#include <iostream>

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
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#include <utils/utils.h>
#include <utils/model.h>
#include <utils/camera.h>
#include <utils/display.h>
#include <utils/waterframe_buffers.h>
#include <utils/gui_texture.h>
#include <utils/renderer/illumination_renderer.h>
#include <utils/renderer/background_renderer.h>
#include <utils/renderer/water_renderer.h>
#include <utils/renderer/gui_renderer.h>

int main () {
    std::cout << "Starting GLFW context" << std::endl;

    GLFWwindow* window = createDisplay();
    
    // // Enable Z test
    glCall(glEnable(GL_DEPTH_TEST));

    // The clear color for the frame buffer
    glCall(glClearColor(0.26f, 0.46f, 0.98f, 1.0f));

    // Projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 10000.0f);
    
    // View matrix
    glm::mat4 view = glm::mat4(1.0f);

    IlluminationRenderer illumination_renderer (projection);
    BackgroundRenderer background_renderer (projection);
    WaterRenderer water_renderer (projection, WIDTH, HEIGHT);

    Model fountainModel("meshes/ball_fountain.obj");
    Model bgModel("meshes/cube.obj");

    vector<WaterTile> waters = {WaterTile(0, -4.8, -0.5)};

    textureCube = LoadTextureCube("textures/skybox/");
    textures.push_back(LoadTexture("textures/terrain.png"));
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        prepareDisplay(wireframe);
        
        // Apply FPS camera movements
        apply_camera_movements();

        glCall(glEnable(GL_CLIP_DISTANCE0));

        water_renderer.getFbos().bindReflectionFrameBuffer();
        GLfloat distance = 2 * (camera.position.y + 0.5);
        camera.position.y -= distance;
        camera.pitch = -camera.pitch;
        glCall(illumination_renderer.render(fountainModel, textures[0], camera, glm::vec4(0, 1, 0, 0.5 + 1)));
        glCall(background_renderer.render(bgModel, textureCube, camera));
        camera.position.y += distance;
        camera.pitch = -camera.pitch;

        water_renderer.getFbos().bindRefractionFrameBuffer();
        glCall(illumination_renderer.render(fountainModel, textures[0], camera, glm::vec4(0, -1, 0, -0.5)));
        glCall(background_renderer.render(bgModel, textureCube, camera));

        glCall(glDisable(GL_CLIP_DISTANCE0));
        water_renderer.getFbos().unbindCurrentFrameBuffer();
        glCall(illumination_renderer.render(fountainModel, textures[0], camera, glm::vec4(0, -1, 0, 100000)));
        glCall(background_renderer.render(bgModel, textureCube, camera));

        glm::vec3 lightColour;
        lightColour.x = illumination_renderer.getLightColor()[0];
        lightColour.y = illumination_renderer.getLightColor()[1];
        lightColour.z = illumination_renderer.getLightColor()[2];
        glCall(water_renderer.render(waters, camera, deltaTime, illumination_renderer.getLightPos()[0], lightColour));

        glfwSwapBuffers(window);
    }

    water_renderer.getFbos().cleanUp();
    illumination_renderer.cleanUp();
    background_renderer.cleanUp();
    water_renderer.cleanUp();

    glfwTerminate();
    return 0;
}









