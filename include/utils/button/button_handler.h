#ifndef BUTTON_HANDLER
#define BUTTON_HANDLER

using namespace std;

#include <vector>
#include <unordered_map>

#include <utils/gui/gui_texture.h>

#include <utils/button/button.h>

class ButtonHandler {
private:
    
    vector <Button*> buttons;

public:
    ButtonHandler () {}

    void update () {
        for (Button* button: buttons)
            button->checkHover();
    }

    void registerButton (Button* button) {
        buttons.push_back(button);
    }

    vector<GuiTexture> getGuiTextures () {
        vector <GuiTexture> guiTextures;
        for (Button* b : buttons) {
            guiTextures.push_back(b->getGuiTexture());
        }
        
        return guiTextures;
    }

    vector<Button*> getButtons () {
        return this->buttons;
    }

    Button* getButton (int id) {
        return this->buttons.at(id);
    }
};

#endif