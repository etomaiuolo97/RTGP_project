#ifndef BUTTON_INTERFACE
#define BUTTON_INTERFACE

#include <glm/glm.hpp>

using namespace std;

#include <utils/gui/gui_texture.h>

#include <vector>

class ButtonInterface {
public:
    virtual void onClick () {};
    virtual void whileHover () {};
    virtual void startHover () {};
    virtual void stopHover () {};
    virtual void checkHover () {};
    virtual void playHoverAnimation (float scalFactor) {};
    virtual void playerClickAnimation (float scaleFactor) {};
    virtual void hide (vector<GuiTexture> guiTextures) {};
    virtual void show (vector<GuiTexture> guiTextures) {};
    virtual void reopen (vector<GuiTexture> guiTextures) {};
};

#endif