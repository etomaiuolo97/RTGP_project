// Code adapted from Java to Cpp from:
// https://www.youtube.com/watch?v=21UsMuFTN0k&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=2

#ifndef WATER_FRAMEBUFFER
#define WATER_FRAMEBUFFER

#include <glad/glad.h>

#include <utils/system/utils.h>

class WaterFrameBuffers {
protected:
    static const int REFLECTION_WIDTH = 800;
    static const int REFLECTION_HEIGHT = 600;

    static const int REFRACTION_WIDTH = 800;
    static const int REFRACTION_HEIGHT = 600;

private:

    GLint screenWidth, screenHeight;

    GLuint reflectionFrameBuffer;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;

    GLuint refractionFrameBuffer;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;

    GLuint createFrameBuffer() {
        GLuint frameBuffer;
        // Generate a new framebuffer and put the its id in the frameBuffer variable
        glCall(glGenFramebuffers(1, &frameBuffer));

        glCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));

        // Tell OpenGL which color attachment render in the current FBO
        glCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
        return frameBuffer;
    }

    void initializeReflectionFrameBuffer() {
        this->reflectionFrameBuffer = this->createFrameBuffer();
        this->reflectionTexture = this->createTextureAttachment(this->REFLECTION_WIDTH, this->REFLECTION_HEIGHT);
        this->reflectionDepthBuffer = this->createDepthBufferAttachment(this->REFLECTION_WIDTH, this->REFLECTION_HEIGHT);
        this->unbindCurrentFrameBuffer();
    }

    void initializeRefractionFrameBuffer() {
        this->refractionFrameBuffer = this->createFrameBuffer();
        this->refractionTexture = this->createTextureAttachment(this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
        this->refractionDepthTexture = this->createDepthTextureAttachment(this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
        this->unbindCurrentFrameBuffer();
    }

    void bindFrameBuffer(GLuint frameBuffer, GLint width, GLint height) {
        // Make sure the texture isn't bound
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
        // Bind the framebuffer with the frameBuffer ID
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
        // Change the viewport to the framebuffer resolution
        glCall(glViewport(0, 0, width, height));
    }

    GLuint createTextureAttachment(GLint width, GLint height) {
        GLuint texture;
        glCall(glGenTextures(1, &texture));
        glCall(glBindTexture(GL_TEXTURE_2D, texture));
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLbyte *) nullptr));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

        // Add the texture attachment to the currently bound FBO and set it to the color attachment 0
        // Since no mip map are generated the last parameter is set to 0
        glCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0));
        return texture;
    }

    GLuint createDepthTextureAttachment(GLint width, GLint height) {
        GLuint texture;
        glCall(glGenTextures(1, &texture));
        glCall(glBindTexture(GL_TEXTURE_2D, texture));
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (GLbyte*) nullptr));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

        // Add the texture attachment to the currently bound FBO and set it to the depth attachment
        glCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0));
        return texture;
    }

    GLuint createDepthBufferAttachment(GLint width, GLint height) {
        GLuint depthBuffer;

        // The non-texture attachments are render buffer
        glCall(glGenRenderbuffers(1, &depthBuffer));
        glCall(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
        // Tell openGL to store depth information in the render buffer
        glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
        glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));
        return depthBuffer;
    }

public:
    WaterFrameBuffers(){}

    // Call then loading the game
    WaterFrameBuffers(GLint screenWidth, GLint screenHeight) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        initializeReflectionFrameBuffer();
        initializeRefractionFrameBuffer();
    }

    // Get the resulting reflection texture
    GLuint getReflectionTexture () {
        return this->reflectionTexture;
    }

    // Get the resulting refraction texture
    GLuint getRefractionTexture () {
        return this->refractionTexture;
    }

    // Get the resulting depth texture
    GLuint getRefractionDepthTexture () {
        return this->refractionDepthTexture;
    }

    // Call when closing the game
    void cleanUp() {
        glCall(glDeleteFramebuffers(1, &this->reflectionFrameBuffer));
        glCall(glDeleteTextures(1, &this->reflectionTexture));
        glCall(glDeleteRenderbuffers(1, &this->reflectionDepthBuffer));

        glCall(glDeleteFramebuffers(1, &this->refractionFrameBuffer));
        glCall(glDeleteTextures(1, &this->refractionTexture));
        glCall(glDeleteTextures(1, &this->refractionDepthTexture));
    }

    void bindReflectionFrameBuffer() {
        bindFrameBuffer(this->reflectionFrameBuffer, this->REFLECTION_WIDTH, this->REFLECTION_HEIGHT);
    }

    void bindRefractionFrameBuffer() {
        bindFrameBuffer(this->refractionFrameBuffer, this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
    }

    void unbindCurrentFrameBuffer() {
        // Switch back to the default framebuffer setting it to 0
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glCall(glViewport(0, 0, this->screenWidth, this->screenHeight));
    }
};

#endif