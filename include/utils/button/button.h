#ifndef BUTTON
#define BUTTON

using namespace std;

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <utils/button/button_interface.h>
#include <utils/system/display.h>

class Button : public ButtonInterface {
private:
    GuiTexture guiTexture;
    glm::vec2 position, scale;
    glm::vec4 color;
    GLboolean isHidden;
    GLboolean isHovering;
    GLboolean onClickStart;

    int lastTextureIndex;
    int buttonId;

    void startRender (vector<GuiTexture> guiTextures) {
        lastTextureIndex = guiTextures.size();
        guiTextures.push_back(guiTexture);
    }

    void stopRender (vector<GuiTexture> guiTextures) {
        guiTextures.erase(guiTextures.begin() + lastTextureIndex);
    }

    void initialize (GuiTexture texture, int buttonId) {
        this->guiTexture = texture;
        this->position = texture.getPosition();
        this->scale = texture.getScale();
        this->color = glm::vec4(1, 1, 1, 1);
        this->isHidden = GL_FALSE;
        this->isHovering = GL_FALSE;
        this->onClickStart = GL_FALSE;

        this->buttonId = buttonId;
    }

    void onClick (){
        switch (buttonId){
            case 0:
                fountainIndex = (fountainIndex + 1) % numFountains;
                break;
            case 1:
                night = setNight(night);
                break;

        }    
    }

public:
    Button (GuiTexture texture, int buttonId) {
        this->initialize(texture, buttonId);
    }

    Button (GuiTexture texture, glm::vec4 color, int buttonId) {
        this->initialize(texture, buttonId);
        this->color = color;
    }

    void checkHover () {
        if (!this->isHidden) {
            glm::vec2 location = guiTexture.getPosition();
            glm::vec2 scale = guiTexture.getScale();
            glm::vec2 mouseCoords = getNormalizedMouseCoordinates(xCursor, yCursor);

            if (location.y + scale.y > -mouseCoords.y && location.y - scale.y < -mouseCoords.y 
                    && location.x + scale.x > mouseCoords.x && location.x - scale.x < mouseCoords.x) {
                whileHover();

                if (!this->isHovering) {
                    isHovering = true;
                    startHover();
                }
                
                if (isClicked) {
                    playerClickAnimation(0.01f);
                    if (isReleased) {
                        onClick();
                        isReleased = false;
                        isClicked = false;
                        startHover();
                    }
                }
            }
            else {
                if (this->isHovering) {
                    isHovering = GL_FALSE;
                    stopHover();
                }
            }
        }
    }

    void startHover () {
        playHoverAnimation(0.02f);
    }

    void stopHover () {
        guiTexture.setScale(this->scale);
    }

    void playHoverAnimation (GLfloat scaleFactor) {
        guiTexture.setScale(glm::vec2(scale.x + scaleFactor, scale.y + scaleFactor));
    }

    void playerClickAnimation (GLfloat scaleFactor) {
        guiTexture.setScale(glm::vec2(scale.x - (scaleFactor * 2), scale.y - (scaleFactor * 2)));
    }

    void hide (vector<GuiTexture> guiTextures) {
        stopRender(guiTextures);
        this->isHidden = GL_TRUE;
    }

    void show (vector<GuiTexture> guiTextures) {
        startRender(guiTextures);
        this->isHidden = GL_FALSE;
    }

    void reopen (vector<GuiTexture> guiTextures) {
        hide(guiTextures);
        show(guiTextures);
    }

    GLboolean getIsHovering () {
        return this->isHovering;
    }

    GLboolean getIsHidden () {
        return this->isHidden;
    }

    GuiTexture getGuiTexture () {
        return this->guiTexture;
    }

    glm::vec4 getColor () {
        return this->color;
    }

    glm::vec2 getScale () {
        return this->scale;
    }

    glm::vec2 getPosition () {
        return this->position;
    }

    void setColor (glm::vec4 color) {
        this->color = color;
    }

    void setScale (glm::vec2 scale) {
        this->scale = scale;
    }

    void setPosition (glm::vec2 position) {
        this->position = position;
    }
};

#endif