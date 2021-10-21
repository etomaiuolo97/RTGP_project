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

#include <utils/system/utils.h>
#include <utils/system/model.h>
#include <utils/system/camera.h>
#include <utils/system/display.h>

#include <utils/model/model_renderer.h>

#include <utils/background/background_renderer.h>

#include <utils/water/water_renderer.h>

#include <utils/particle/particle_renderer.h>
#include <utils/particle/particle_framebuffers.h>

int main () {
    std::cout << "Starting GLFW context" << std::endl;

    GLFWwindow* window = createDisplay();
    
    // Enable Z test
    glCall(glEnable(GL_DEPTH_TEST));

    // The clear color for the frame buffer
    glCall(glClearColor(0.26f, 0.46f, 0.98f, 1.0f));

    // Projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 10000.0f);
    
    // Renderers
    ModelRenderer model_renderer (projection);
    BackgroundRenderer background_renderer (projection);
    WaterRenderer water_renderer (projection, WIDTH, HEIGHT);
    ParticleRenderer particle_renderer (projection);

    // Models
    Model fountainModel("meshes/ball_fountain.obj");
    Model bgModel("meshes/cube.obj");

    camera.setObjPosition(model_renderer.getPosition());

    // Textures
    textureCube = LoadTextureCube("textures/skybox/");
    
    Texture model_texture;
    model_texture.id = LoadTexture("textures/fountain/fountain_tex.png", true);
    model_texture.shineDamper = 90.0f;
    model_texture.reflectivity = 1.0f;
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        prepareDisplay(wireframe);
        
        // Apply FPS camera movements
        apply_camera_movements();

        // Render the reflection of the water
        water_renderer.bindReflectionFrameBuffer();

        glCall(glEnable(GL_CLIP_DISTANCE0));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));

        GLfloat distance = 2 * (camera.getPosition().y - water_renderer.getWaterHeight());
        glm::vec3 cameraPos = camera.getPosition();

        cameraPos.y -= distance;
        camera.setPosition(cameraPos);
        camera.setPitch(-camera.getPitch());

        background_renderer.render(bgModel, textureCube, camera);
        model_renderer.render(fountainModel, model_texture, camera, 
                glm::vec4(0, 1, 0, -water_renderer.getWaterHeight()));

        cameraPos.y += distance;
        camera.setPosition(cameraPos);
        camera.setPitch(-camera.getPitch());

        // Render the refraction of the water
        water_renderer.bindRefractionFrameBuffer();

        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        model_renderer.render(fountainModel, model_texture, camera, glm::vec4(0, -1, 0, water_renderer.getWaterHeight()));
        background_renderer.render(bgModel, textureCube, camera);
        water_renderer.unbindCurrentFrameBuffer();

        glCall(glDisable(GL_CLIP_DISTANCE0));

        // Render the reflection of the particle
        particle_renderer.bindReflectionFrameBuffer();

        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        camera.setPitch(-camera.getPitch());
        camera.setYaw(camera.getYaw() + 180.0f);
        glm::vec3 objPosition = camera.getObjPosition();
        objPosition.y += distance;
        camera.setPosition(objPosition);
        
        background_renderer.render(bgModel, textureCube, camera);

        camera.setPitch(-camera.getPitch());
        camera.setYaw(camera.getYaw() - 180.0f);
        camera.setPosition(cameraPos);

        // Render the refraction of the particle
        particle_renderer.bindRefractionFrameBuffer();

        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        model_renderer.render(fountainModel, model_texture, camera, glm::vec4(0, -1, 0, 100000));
        background_renderer.render(bgModel, textureCube, camera);
        water_renderer.render(camera, deltaTime, model_renderer.getLightPos(), model_renderer.getLightColor());
        particle_renderer.unbindCurrentFrameBuffer();

        // Render the scene
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        model_renderer.render(fountainModel, model_texture, camera, glm::vec4(0, -1, 0, 100000));
        background_renderer.render(bgModel, textureCube, camera);
        water_renderer.render(camera, deltaTime, model_renderer.getLightPos(), model_renderer.getLightColor());

        particle_renderer.render(deltaTime, camera, model_renderer.getLight(), textureCube);

        glfwSwapBuffers(window);
    }

    particle_renderer.cleanUp();
    model_renderer.cleanUp();
    background_renderer.cleanUp();
    water_renderer.cleanUp();

    glfwTerminate();
    return 0;
}









