#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec3 outColor;


out vec4 fragColor;

float ka =0.8;
float kd =0.8;
float ks =0.8;
vec3 cAmbient = vec3(0);
vec3 lightDir = vec3(1,-1,1);

//uniform vec3 posCam;

void main() {
    vec3 total = vec3(0.0,0.0,0.0);
    //ambient

    total += ka*outColor;


    vec3 normWorld = vec3(normalize(worldNormal));
    vec3 intersectToSource = -normalize(lightDir);
    vec3 intensity = vec3(1.f);
    float ndotl = dot(normWorld,intersectToSource);
    ndotl = min(1.f,max(ndotl,0.f));
    total += kd*intensity*outColor*ndotl;
    fragColor = vec4(clamp(total,0.0,1.0),1.f);
}
