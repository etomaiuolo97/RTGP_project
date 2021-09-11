// #include <glad.h>
#ifndef WATER_FRAMEBUFFER
#define WATER_FRAMEBUFFER

class WaterFrameBuffers {
protected:
    static const int REFLECTION_WIDTH = 320;
    static const int REFLECTION_HEIGHT = 180;

    static const int REFRACTION_WIDTH = 1280;
    static const int REFRACTION_HEIGHT = 720;

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
        glCall(glGenFramebuffers(1, &frameBuffer));
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
        glCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
        return frameBuffer;
    }

    void initializeReflectionFrameBuffer() {
        this->reflectionFrameBuffer = this->createFrameBuffer();
        this->reflectionTexture = this->createTextureAttachment(this->REFLECTION_WIDTH, this->REFLECTION_HEIGHT);
        this->reflectionDepthBuffer = this->createDepthBufferAttachment(this->REFLECTION_WIDTH, this->REFLECTION_HEIGHT);
        std::cout << "Reflection buffer" << std::endl;
        this->unbindCurrentFrameBuffer();
    }

    void initializeRefractionFrameBuffer() {
        this->refractionFrameBuffer = this->createFrameBuffer();
        this->refractionTexture = this->createTextureAttachment(this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
        this->refractionDepthTexture = this->createDepthTextureAttachment(this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
        std::cout << "Refraction buffer" << std::endl;
        this->unbindCurrentFrameBuffer();
    }

    void bindFrameBuffer(GLuint frameBuffer, GLint width, GLint height) {
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
        glCall(glViewport(0, 0, width, height));
    }

    GLuint createTextureAttachment(GLint width, GLint height) {
        GLuint texture;
        glCall(glGenTextures(1, &texture));
        glCall(glBindTexture(GL_TEXTURE_2D, texture));
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLbyte *) nullptr));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
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
        glCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0));
        return texture;
    }

    GLuint createDepthBufferAttachment(GLint width, GLint height) {
        GLuint depthBuffer;
        glCall(glGenRenderbuffers(1, &depthBuffer));
        glCall(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
        glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
        glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));
        return depthBuffer;
    }

public:
    WaterFrameBuffers(){}

    WaterFrameBuffers(GLint screenWidth, GLint screenHeight) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        initializeReflectionFrameBuffer();
        initializeRefractionFrameBuffer();
    }

    GLint getReflectionTexture () {
        return this->reflectionTexture;
    }

    GLint getRefractionTexture () {
        return this->refractionTexture;
    }

    GLint getRefractionDepthTexture () {
        return this->refractionDepthTexture;
    }

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
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glCall(glViewport(0, 0, this->screenWidth, this->screenHeight));
    }
};

#endif