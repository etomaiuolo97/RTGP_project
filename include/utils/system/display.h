#ifndef DISPLAY
#define DISPLAY

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include <utils/system/camera.h>

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

GLboolean wireframe = GL_FALSE;

bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX, lastY;

// To manage the mouse position in the first frame
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));

void key_callback (GLFWwindow* window, int key, int scancode, int action, int mode) {
    GLuint new_subroutine;

    // If ESC is pressed, we close the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    // Keep trace of the pressed keys
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

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

void mouse_callback (GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
}

GLFWwindow* createDisplay() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project", nullptr, nullptr);

    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return nullptr;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    return window;
}

void prepareDisplay(GLboolean wireframe){
    glfwPollEvents();

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

#endif