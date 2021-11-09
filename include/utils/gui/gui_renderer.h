#pragma once

#ifndef GUI_RENDERER
#define GUI_RENDERER

#include "utils/system/renderer.h"
#include "utils/system/model.h"
#include "utils/gui/gui_texture.h"
#include "utils/gui/gui_shader.h"

class GuiRenderer : public Renderer{
private:
    GuiShader shader;
    Model quad;

public:

    GuiRenderer ()
        :Renderer(glm::mat4(1.0f)){
        quad = Model ("meshes/quad.obj");
    }

    void render(vector<GuiTexture> guis, Camera & camera) {
        shader.start();
        for (GuiTexture gui: guis){
            glCall(glActiveTexture(GL_TEXTURE0));
            glCall(glBindTexture(GL_TEXTURE_2D, gui.getTexture()));
            shader.combineTextures();

            glm::mat4 matrix = createTransformationMatrix(gui.getPosition(), gui.getScale());
            shader.loadModelMatrix(matrix);
            quad.Draw();
        }
        shader.stop();
    }

    void cleanUp(){
        shader.cleanUp();
    }
};

#endif