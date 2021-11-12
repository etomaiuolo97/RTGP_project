#version 410

in vec2 textureCoords;

out vec4 o_Color;

uniform sampler2D u_GuiTexture;

void main(void){

	o_Color = texture(u_GuiTexture, textureCoords);

}