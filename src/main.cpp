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

#include <utils/shader.h>
#include <utils/model.h>
#include <utils/camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// number of lights in the scene
#define NR_LIGHTS 3

//Window dimensions
GLuint screenWidth = 800, screenHeight = 600;

// Callback functions for keyboard events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// If one of the WASD keys is pressed, we call the corresponding method of the Camera class
void apply_camera_movements();

// Setup of Shader Programs for the shader used in the application
void SetupShaders(int program);

// Delete Shader Programs when application ends
// void DeleteShaders();

// Print on console the name of the current shader
void PrintCurrentShader(int shader);

// Parameters for time calculation (for animations)
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat orientationY = 0.5f;

// Initialize an array of booleans for each keyboard key
bool keys[1024];

// Previous mouse position
GLfloat lastX, lastY;

// To manage the mouse position in the first frame
bool firstMouse = true;

// Index of the currente shader (= 0  in the beginning)
GLuint current_program = 0;

// Vector for all the Shader Programs used and swapped in the application
vector<std::string> shaders;

// Activate/deactivate wireframe rendering
GLboolean wireframe = GL_FALSE;

// Camera with an initial position
// GL_TRUE = camera fixed to the ground
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f), GL_TRUE);

// Uniforms to pass to shaders
GLfloat planeMaterial[] = {0.0f, 0.5f, 0.0f};

glm::vec3 lightPositions [] = {
    glm::vec3(5.0f, 10.0f, 10.0f),
    glm::vec3(-5.0f, 10.0f, 10.0f),
    glm::vec3(5.0f, 10.0f, -10.0f)
};

GLfloat diffuseColor [] = {1.0f, 0.0f, 0.0f};
GLfloat specularColor [] = {1.0f, 1.0f, 1.0f};
GLfloat ambientColor [] = {0.1f, 0.1f, 0.1f};

GLfloat Kd = 0.5f;
GLfloat Ks = 0.4f;
GLfloat Ka = 0.1f;

GLfloat shininess = 25.0f;

int main () {
    std::cout << "Starting GLFW context" << std::endl;

    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Project", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Enable Z test
    glEnable(GL_DEPTH_TEST);

    // The clear color for the frame buffer
    glClearColor(0.26f, 0.46f, 0.98f, 1.0f);

    Shader illumination_shader = Shader("illumination_model.vert", "illumination_model.frag");
    SetupShaders(illumination_shader.Program);
    PrintCurrentShader(current_program);

    Model fountainModel("../meshes/ball_fountain.obj");
    Model planeModel("../meshes/plane.obj");
    
    // Projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100000.0f);
    
    // View matrix
    glm::mat4 view = glm::mat4(1.0f);

    // Model and Normal transformation matrices for the objects in the scene
    glm::mat4 fountainModMatrix = glm::mat4(1.0f);
    glm::mat3 fountainNorMatrix = glm::mat3(1.0f);
    glm::mat4 planeModMatrix = glm::mat4(1.0f);
    glm::mat3 planeNorMatrix = glm::mat3(1.0f);

    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check I/O events
        glfwPollEvents();
        
        // Apply FPS camera movements
        apply_camera_movements();
        view = camera.GetViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        // Plane render
        illumination_shader.Use();
        GLuint index = glGetSubroutineIndex(illumination_shader.Program, GL_FRAGMENT_SHADER, "BlinnPhong_ML");
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        // // Determine the position in the Shader Program of the uniform variables
        GLint matDiffuseLocation = glGetUniformLocation(illumination_shader.Program, "diffuseColor");
        GLint matAmbientLocation = glGetUniformLocation(illumination_shader.Program, "ambientColor");
        GLint matSpecularLocation = glGetUniformLocation(illumination_shader.Program, "specularColor");
        GLint kaLocation = glGetUniformLocation(illumination_shader.Program, "Ka");
        GLint kdLocation = glGetUniformLocation(illumination_shader.Program, "Kd");
        GLint ksLocation = glGetUniformLocation(illumination_shader.Program, "Ks");
        GLint shineLocation = glGetUniformLocation(illumination_shader.Program, "shininess");

        // // Uniform variables
        glUniform3fv(matAmbientLocation, 1, ambientColor);
        glUniform3fv(matSpecularLocation, 1, specularColor);
        glUniform1f(shineLocation, shininess);

        // // Specular component
        glUniform1f(kaLocation, 0.0f);
        glUniform1f(kdLocation, 0.6f);
        glUniform1f(ksLocation, 0.0f);

        // // Projection and view matrices to Shader Program
        glUniformMatrix4fv(glGetUniformLocation(illumination_shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(illumination_shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));

        for (GLuint i = 0; i < NR_LIGHTS; i++) {
            string number = to_string(i);
            glUniform3fv(glGetUniformLocation(illumination_shader.Program, ("lights[" + number + "]").c_str()), 1, glm::value_ptr(lightPositions[i]));
        }

        glUniform3fv(matDiffuseLocation, 1, planeMaterial);

        planeModMatrix = glm::mat4(1.0f);
        planeNorMatrix = glm::mat3(1.0f);

        planeModMatrix = glm::translate(planeModMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
        planeModMatrix = glm::scale(planeModMatrix, glm::vec3(10.0f, 1.0f, 10.0f));

        planeNorMatrix = glm::inverseTranspose(glm::mat3(view * planeModMatrix));

        glUniformMatrix4fv(glGetUniformLocation(illumination_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(planeModMatrix));
        glUniformMatrix3fv(glGetUniformLocation(illumination_shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(planeNorMatrix));

        planeModel.Draw();

        // Fountain
        index = glGetSubroutineIndex(illumination_shader.Program, GL_FRAGMENT_SHADER, shaders[current_program].c_str());
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);

        glUniform3fv(matDiffuseLocation, 1, diffuseColor);
        glUniform1f(ksLocation, Ka);
        glUniform1f(ksLocation, Kd);
        glUniform1f(ksLocation, Ks);

        fountainModMatrix = glm::mat4(1.0f);
        fountainNorMatrix = glm::mat3(1.0f);
        fountainModMatrix = glm::translate(fountainModMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        fountainModMatrix = glm::rotate(fountainModMatrix, glm::radians(orientationY), glm::vec3(1.0f, 0.0f, 0.0f));
        fountainModMatrix = glm::scale(fountainModMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
        fountainNorMatrix = glm::inverseTranspose(glm::mat3(view * fountainModMatrix));
        glUniformMatrix4fv(glGetUniformLocation(illumination_shader.Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(fountainModMatrix));
        glUniformMatrix3fv(glGetUniformLocation(illumination_shader.Program, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(fountainNorMatrix));

        fountainModel.Draw();

        glfwSwapBuffers(window);
    }

    illumination_shader.Delete();
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
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