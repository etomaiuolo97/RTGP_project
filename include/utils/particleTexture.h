#pragma once

class particleTexture
{
private:
    int textureID;
    int numberRows;


public:
    particleTexture(int textureID, int numberRows){
        this->textureID = textureID;
        this->numberRows = numberRows;
    }

    int getTextureID(){
        return textureID;
    }

    int getNumberRows(){
        return numberRows;
    }
    
    particleTexture();
};
