// #include <glad.h>

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
    GLuint refractionDepthBuffer;

    GLuint createFrameBuffer() {
        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
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
        this->refractionDepthBuffer = this->createDepthBufferAttachment(this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
        this->unbindCurrentFrameBuffer();
    }

    void bindFrameBuffer(GLint frameBuffer, GLint width, GLint height) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);
    }

    GLuint createTextureAttachment(GLint width, GLint height) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLbyte *) nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
        return texture;
    }

    GLuint createDepthTextureAttachment(GLint width, GLint height) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (GLbyte*) nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
        return texture;
    }

    GLuint createDepthBufferAttachment(GLint width, GLint height) {
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        return depthBuffer;
    }

public:
    WaterFrameBuffers(GLint screenWidth, GLint screenHeight) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        initializeReflectionFrameBuffer();
        initializeRefractionFrameBuffer();
    }

    void cleanUp() {
        glDeleteFramebuffers(1, &this->reflectionFrameBuffer);
        glDeleteTextures(1, &this->reflectionTexture);
        glDeleteRenderbuffers(1, &this->reflectionDepthBuffer);

        glDeleteFramebuffers(1, &this->refractionFrameBuffer);
        glDeleteTextures(1, &this->refractionTexture);
        glDeleteRenderbuffers(1, &this->refractionDepthBuffer);
    }

    void bindReflectionFrameBuffer() {
        bindFrameBuffer(this->reflectionFrameBuffer, this->REFLECTION_WIDTH, this->REFLECTION_HEIGHT);
    }

    void bindRefractionFrameBuffer() {
        bindFrameBuffer(this->refractionFrameBuffer, this->REFRACTION_WIDTH, this->REFRACTION_HEIGHT);
    }

    void unbindCurrentFrameBuffer() {
        glBindRenderbuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, this->screenWidth, this->screenHeight);
    }
};