#version 410 core

layout (location = 0) out vec4 o_Color;

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec4 u_Color;

uniform samplerCube u_TCube;
uniform vec3 u_CameraPosition;
uniform vec3 u_PointLightPosition;

void main()
{
	// Vertex-Camera vector in world coordinates
    // worldNormal = vec3(worldNormal.x, worldNormal.y, -worldNormal.z);
    vec3 N = normalize(vec3(worldNormal.x, worldNormal.y, -worldNormal.z));
    vec3 V = normalize(worldPosition.xyz - u_CameraPosition);

    // Reflection vector of I w.r.t. normal
    vec3 R = reflect(V, normalize(worldNormal));
    vec3 L = normalize(worldPosition.xyz - u_PointLightPosition);
    vec3 H = normalize(L + V);

    // Sample the texture cube using the components of reflection vector as texture coordinates
    vec4 reflectedCol =  texture(u_TCube, R);
	
	float Eta = 1.0f;
	vec3 refractDir[3];
    refractDir[0] = refract(V, N, Eta);
    refractDir[1] = refract(V, N, Eta * 0.99);
    refractDir[2] = refract(V, N, Eta * 0.98);

	vec4 refractedCol = vec4(1.0);
    refractedCol.r = texture(u_TCube, refractDir[0]).r;
    refractedCol.g = texture(u_TCube, refractDir[1]).g;
    refractedCol.b = texture(u_TCube, refractDir[2]).b;

	float mFresnelPower = 10.0f;
    float F0 = ((1.0 - Eta) * (1.0 - Eta)) / ((1.0 + Eta) * (1.0 + Eta));
    float ratio = F0 + (1.0 - F0) * pow(1.0 - max(dot(V, H), 0.0), mFresnelPower);

    vec4 out_Color = mix(refractedCol, reflectedCol, clamp(ratio, 0.0, 1.0));
    o_Color = vec4(out_Color.xyz, 0.5);
}