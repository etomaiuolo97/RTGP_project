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
#include <utils/system/camera.h>
#include <utils/system/display.h>

#include <utils/model/model_renderer.h>
#include <utils/model/model.h>

#include <utils/background/background_renderer.h>

#include <utils/water/water_renderer.h>

#include <utils/particle/particle_renderer.h>
#include <utils/particle/particle_framebuffers.h>

#include <utils/gui/gui_renderer.h>

#include <utils/button/button.h>
#include <utils/button/button_handler.h>
#include <utils/button/button_function.h>

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
    ModelRenderer model_renderer (projection,night);
    BackgroundRenderer background_renderer (projection);
    WaterRenderer water_renderer (projection, WIDTH, HEIGHT);
    ParticleRenderer particle_renderer (projection);
    GuiRenderer gui_renderer;

    // Models
    Model bgModel("meshes/cube.obj");
    // List of fountains
    vector<Model> fountain_models;
    fountain_models.push_back(Model("meshes/fountain_ball.obj", glm::vec3(0.0f, -3.0f, -5.0f), glm::vec3(-90.0f, 0.0f, 0.0f), 1.0f));
    fountain_models.push_back(Model("meshes/fountain_child.obj", glm::vec3(0.0f, -3.0f, -5.0f), glm::vec3(-90.0f, 0.0f, 180.0f), 0.08f));
    fountain_models.push_back(Model("meshes/fountain_classic.obj", glm::vec3(0.0f, -4.0f, -5.0f), glm::vec3(-90.0f, 0.0f, 0.0f), 0.15f));
    // List of water models
    vector<Model> water_models;
    water_models.push_back(Model("meshes/circle.obj", glm::vec3(0.0f, -0.1f, -5.0f), glm::vec3(0.0f), 2.8f));
    water_models.push_back(Model("meshes/circle.obj", glm::vec3(0.0f, -1.0f, -5.0f), glm::vec3(0.0f), 3.0f));
    water_models.push_back(Model("meshes/exagon.obj", glm::vec3(0.0f, -1.5f, -5.0f), glm::vec3(0.0f), 3.0f));
    // List of begin of fountain
    vector<particle_prop_t> particle_props;

    particle_prop_t temp;
    temp.position = glm::vec3(0.0f, 4.5f, -5.0f);
    temp.water_height = water_models[0].getPosition().y + 0.1f;
    temp.angle = 90.0f;
    temp.life = 1.5f;

    particle_props.push_back(temp);

    temp.position = glm::vec3(0.0f, 7.5f, -5.0f);
    temp.water_height = water_models[1].getPosition().y + 0.1f;
    temp.angle = 40.0f;
    temp.life = 2.0f;

    particle_props.push_back(temp);

    temp.position = glm::vec3(0.0f, 4.5f, -5.0f);
    temp.water_height = water_models[2].getPosition().y + 0.1f;
    temp.angle = 120.0f;
    temp.life = 1.2f;

    particle_props.push_back(temp);

    numFountains = fountain_models.size();

    camera.setObjPosition(fountain_models[fountainIndex].getPosition());

    // Textures
    textureCube = LoadTextureCube("textures/skybox/");
    
    Texture model_texture;
    model_texture.id = LoadTexture("textures/fountain/fountain_tex.png", true);
    model_texture.shineDamper = 90.0f;
    model_texture.reflectivity = 1.0f;

    GuiTexture btnModelTexture (LoadTexture("textures/button/fountain_button.png", false, true), 
        glm::vec2(-0.75f, -0.75f), glm::vec2(0.0f, 0.0f), glm::vec2(0.12f, 0.15f));
    Button btnModel (btnModelTexture, 0);

    GuiTexture btnDayNightTexture (LoadTexture("textures/button/day_night_button.png", false, true), 
        glm::vec2(0.0f, -0.75f), glm::vec2(0.0f, 0.0f), glm::vec2(0.12f, 0.15f));
    Button btnDayNight (btnDayNightTexture, 1);

    GuiTexture btnSceneTexture (LoadTexture("textures/button/scene_button.png", false, true), 
        glm::vec2(0.75f, -0.75f), glm::vec2(0.0f, 0.0f), glm::vec2(0.12f, 0.15f));
    Button btnScene (btnSceneTexture, 2);

    ButtonHandler btn_handler;
    btn_handler.registerButton(&btnModel);
    btn_handler.registerButton(&btnDayNight);
    btn_handler.registerButton(&btnScene);
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        prepareDisplay(wireframe);
        
        // Apply FPS camera movements
        apply_camera_movements();

        btn_handler.update();

        if(night){
            model_renderer.setLightColor(glm::vec3(0.6f, 0.6f, 0.6f));
        }else{
            model_renderer.setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
        }

        // Render the reflection of the water
        water_renderer.bindReflectionFrameBuffer();

        glCall(glEnable(GL_CLIP_DISTANCE0));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));

        GLfloat distance = 2 * (camera.getPosition().y - water_models[fountainIndex].getPosition().y);
        glm::vec3 cameraPos = camera.getPosition();

        cameraPos.y -= distance;
        camera.setPosition(cameraPos);
        camera.setPitch(-camera.getPitch());

        background_renderer.render(bgModel, textureCube, camera);
        model_renderer.render(fountain_models[fountainIndex], model_texture, camera, 
                glm::vec4(0, 1, 0, -water_models[fountainIndex].getPosition().y));

        cameraPos.y += distance;
        camera.setPosition(cameraPos);
        camera.setPitch(-camera.getPitch());

        // Render the refraction of the water
        water_renderer.bindRefractionFrameBuffer();

        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        model_renderer.render(fountain_models[fountainIndex], model_texture, camera, 
                glm::vec4(0, -1, 0, water_models[fountainIndex].getPosition().y));
        background_renderer.render(bgModel, textureCube, camera);

        water_renderer.unbindCurrentFrameBuffer();

        glCall(glDisable(GL_CLIP_DISTANCE0));

        // Render the reflection of the particle
        particle_renderer.bindReflectionFrameBuffer();

        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        camera.setPitch(-camera.getPitch());
        camera.setYaw(camera.getYaw() + 180.0f);
        glm::vec3 objPosition = fountain_models[fountainIndex].getPosition();
        objPosition.y += distance;
        camera.setPosition(objPosition);
        
        background_renderer.render(bgModel, textureCube, camera);

        camera.setPitch(-camera.getPitch());
        camera.setYaw(camera.getYaw() - 180.0f);
        camera.setPosition(cameraPos);

        // Render the refraction of the particle
        particle_renderer.bindRefractionFrameBuffer();

        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        model_renderer.render(fountain_models[fountainIndex], model_texture, camera, glm::vec4(0, -1, 0, 100000));
        background_renderer.render(bgModel, textureCube, camera);
        water_renderer.render(water_models[fountainIndex], camera, deltaTime, model_renderer.getLightPos(), model_renderer.getLightColor());

        particle_renderer.unbindCurrentFrameBuffer();

        // Render the scene
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        background_renderer.render(bgModel, textureCube, camera);
        model_renderer.render(fountain_models[fountainIndex], model_texture, camera, glm::vec4(0, -1, 0, 100000));
        water_renderer.render(water_models[fountainIndex], camera, deltaTime, model_renderer.getLightPos(), model_renderer.getLightColor());
        particle_renderer.render(particle_props[fountainIndex], deltaTime, camera, model_renderer.getLight(), textureCube);
        gui_renderer.render(btn_handler.getGuiTextures(), camera);

        glfwSwapBuffers(window);
    }

    

    particle_renderer.cleanUp();
    model_renderer.cleanUp();
    background_renderer.cleanUp();
    water_renderer.cleanUp();
    gui_renderer.cleanUp();

    glfwTerminate();
    return 0;
}









