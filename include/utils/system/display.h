/* The display class create a display. We have started by the code published by the professor. 
    We added the zoom function and extracted this class from the main.
*/

#ifndef DISPLAY
#define DISPLAY

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include <utils/system/camera.h>

// define paramenter of the window
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;
// boolean to activate/deactivate wireframe rendering
GLboolean wireframe = GL_FALSE;
// this initializes an array of booleans for each keybord key
bool keys[1024];
// parameters for time calculation (for animations)
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// this line stores the previous mouse position to calculate the offset with the current frame
GLfloat lastX, lastY;

// To manage the mouse position in the first frame
bool firstMouse = true;

Camera camera;
bool night=true;

// callback function for keyboard events
void key_callback (GLFWwindow* window, int key, int scancode, int action, int mode) {
    GLuint new_subroutine;

    // If ESC is pressed, it closes the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // 1 : beach, 2 : parc
    if(key == GLFW_KEY_1 && action == GLFW_PRESS){
        std::cout << "\t1 - Beach" << std::endl;
        textureCube = LoadTextureCube("textures/skybox/");
    }else if(key == GLFW_KEY_2 && action == GLFW_PRESS){
        std::cout << "\t2 - Parc" << std::endl;
        textureCube = LoadTextureCube("textures/skybox1/");
    }

    // 3: day , 4 : night
    if(key == GLFW_KEY_3 && action == GLFW_PRESS){
        std::cout << "\t3 - Day" << std::endl;
        textureCube = LoadTextureCube("textures/skyboxDay/");
        night=false;
    }else if(key == GLFW_KEY_4 && action == GLFW_PRESS){
        std::cout << "\t4 - Night" << std::endl;
        textureCube = LoadTextureCube("textures/skyboxNight/");
        night=true;
    }
  
    // Keep trace of the pressed keys
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

// The camera is moved in base of the keyboard arrows and if W or S is pressed
void apply_camera_movements () {
    if (keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(UP, deltaTime);
    
    if (keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
}

/**
 * @brief The display is zoomed and we pass the coordinate of the mouse to calculate the zoom
 * 
 * @param window 
 * @param xoffset 
 * @param yoffset y coordinate of the mouse
 */
void scroll_callback (GLFWwindow* window, double xoffset, double yoffset){
    camera.calculateZoom(yoffset);
}

// Creation of the display
GLFWwindow* createDisplay() {
    // Initialization of OpenGL context using GLFW
    glfwInit();
    // OpenGL specifications required for this application are set. In this case: 4.1 Core
    // We active the debug in glfw
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // it specifies whether the OpenGL context should be forward-compatible. This must only be used 
    // if the requested OpenGL version is 3.0 or above.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // we set if the window is resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // the parameter specifies whether to use full resolution framebuffers on Retina displays. 
    // This is ignored on other platforms.
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project", nullptr, nullptr);
    // this creates the application's window
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    // here the code puts in relation the window and the callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // GLAD tries to load the context set by GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return nullptr;
    }
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    return window;
}

/**
 * @brief Prepare the display.
 * 
 * @param wireframe boolean to activate/disactive wireframe rendering
 */
void prepareDisplay(GLboolean wireframe){
    glfwPollEvents();
    // The rendering mode get set
    if (wireframe)
        // Draw in wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

#endif