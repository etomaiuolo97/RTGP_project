#version 410

in vec2 textureCoords;

out vec4 out_Color;

uniform sampler2D guiTexture;
uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

in vec4 worldPosition;
in vec3 worldNormal;

const float eta = 1.0f;


void main(void){
	
	vec3 N = normalize(worldNormal);

    // Vector from vertex to camera in world coordinates
    vec3 V = normalize(worldPosition.xyz - u_CameraPosition);

	vec3 refractDir[3];
    refractDir[0] = refract(V , N, eta);
    refractDir[1] = refract(V , N, eta * 0.99);
    refractDir[2] = refract(V , N, eta * 0.98);

    vec4 refractedColor = vec4(1.0);
    refractedColor.r = texture(guiTexture,  vec2(refractDir[0].x, refractDir[0].y)).r;
    refractedColor.g = texture(guiTexture,  vec2(refractDir[1].x, refractDir[1].y)).g;
    refractedColor.b = texture(guiTexture,  vec2(refractDir[2].x, refractDir[2].y)).b;


	out_Color = refractedColor;

}