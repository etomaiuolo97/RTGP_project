// Cpp version of the code https://www.youtube.com/watch?v=GmmR37-LBPQ

#ifndef BUTTON
#define BUTTON

using namespace std;

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <utils/system/display.h>

class Button {
private:

    // guiTexture: texture of the button
    GuiTexture guiTexture;
    // position, scale: position and scale of the button
    glm::vec2 position, scale;
    // isHovering: True if the mouse pointer is on the button
    GLboolean isHovering;
    
    
    
    // buttonId: id to set the onClick function:
    //  - 0: model change
    //  - 1: day/night version
    //  - 2: scene change
    int buttonId;

    void initialize (GuiTexture texture, int buttonId) {
        this->guiTexture = texture;
        this->position = texture.getPosition();
        this->scale = texture.getScale();
        this->isHovering = GL_FALSE;

        this->buttonId = buttonId;

        // Initialize the list of cubemaps and the index in case of scene-change button
        switch (buttonId) {
            case 0:
                fountainIndex = 0;
                break;
            case 1:
                isNight = false;
                break;
            case 2:
                cubeTexIndex = 0;
                break;
        }
    }

    void onClick (){
        switch (buttonId){
            case 0:
                // Cycle on the fountain vector index
                fountainIndex = (fountainIndex + 1) % numFountains;
                break;
            case 1:
                isNight = !isNight;
                if (isNight)
                    cubeTexIndex = numCubeTextures / 2;
                else
                    cubeTexIndex = 0;

                break;
            case 2:
                // Cycle on the cubemap vector index
                if (isNight)
                    cubeTexIndex -= numCubeTextures / 2;
                
                cubeTexIndex = (cubeTexIndex + 1) % (numCubeTextures / 2);

                if (isNight)
                    cubeTexIndex += numCubeTextures / 2;
                break;
        }    
    }

public:
    Button (GuiTexture texture, int buttonId) {
        this->initialize(texture, buttonId);
    }

    /**
     * @brief Check if the mouse pointer is on the button.
    */
    void checkHover () {
        glm::vec2 location = guiTexture.getPosition();
        glm::vec2 scale = guiTexture.getScale();

        // Get the normalized mouse coordinates taken from the mouse callback
        glm::vec2 mouseCoords = getNormalizedMouseCoordinates(xCursor, yCursor);

        // Check if the mouse position is within the limit coordinates of the button
        if (location.y + scale.y > -mouseCoords.y && location.y - scale.y < -mouseCoords.y 
                && location.x + scale.x > mouseCoords.x && location.x - scale.x < mouseCoords.x) {
            
            // Change the status of the button
            if (!this->isHovering) {
                isHovering = true;
                startHover();
            }

            // Check if the mouse is clicked
            if (isClicked) {
                playerClickAnimation(0.01f);
                // Do the onClick function only when the mouse button is released
                if (isReleased) {
                    onClick();
                    isReleased = false;
                    isClicked = false;
                    startHover();
                }
            }
        }
        else {
            // Change the status of the button if the mouse is exiting the button
            if (this->isHovering) {
                isHovering = GL_FALSE;
                stopHover();
            }
        }
    }

    /**
     * @brief Zooming the button of scaleFactor of 0.02f
     */
    void startHover () {
        playHoverAnimation(0.02f);
    }

    /**
     * @brief Reset the button size
     */
    void stopHover () {
        guiTexture.setScale(this->scale);
    }

    /**
     * @brief Zooming the button.
     * 
     * @param scaleFactor zoom in factor
     */
    void playHoverAnimation (GLfloat scaleFactor) {
        guiTexture.setScale(glm::vec2(scale.x + scaleFactor, scale.y + scaleFactor));
    }

    /**
     * @brief Reduce the button size
     * 
     * @param scaleFactor zoom out factor
     */
    void playerClickAnimation (GLfloat scaleFactor) {
        guiTexture.setScale(glm::vec2(scale.x - (scaleFactor * 2), scale.y - (scaleFactor * 2)));
    }

    GLboolean getIsHovering () {
        return this->isHovering;
    }

    GuiTexture getGuiTexture () {
        return this->guiTexture;
    }

    glm::vec2 getScale () {
        return this->scale;
    }

    glm::vec2 getPosition () {
        return this->position;
    }

    void setScale (glm::vec2 scale) {
        this->scale = scale;
    }

    void setPosition (glm::vec2 position) {
        this->position = position;
    }
};

#endif