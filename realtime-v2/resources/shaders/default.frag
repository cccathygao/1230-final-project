 #version 330 core

struct Light {
    int type;       //0 = dir, 1 = point, 2 = spot
    vec3 color;
    vec3 function;
    vec3 pos;       // Not applicable to directional lights
    vec3 dir;       // Not applicable to point lights
    float penumbra;      // Only applicable to spot lights, in RADIANS
    float angle;         // Only applicable to spot lights, in RADIANS
};

in vec3 worldPosition;
in vec3 worldNormal;


layout(location=1) out float fragDepth;
layout(location=0) out vec4 fragColor;
uniform float ka;
uniform float kd;
uniform float ks;
uniform vec3 cAmbient;
uniform vec3 cDiffuse;
uniform vec3 cSpecular;
uniform float shininess;
uniform vec3 posCam;


uniform int nlights;

uniform Light lights[8];


void main() {
    vec3 total = vec3(0.0,0.0,0.0);
    //ambient

    total += ka*cAmbient;

    vec3 normWorld = vec3(normalize(worldNormal));
    for(int i=0; i<nlights; i++){
        float fatt = 1.f;
        vec3 intersectToSource = vec3(0.f,0.f,0.f);
        if(lights[i].type == 0){
            intersectToSource = -normalize(lights[i].dir);
        }
        else{
            intersectToSource = normalize(lights[i].pos-worldPosition);
            float dist = distance(lights[i].pos,worldPosition);
            fatt = min(1.f/(lights[i].function[0]+dist*lights[i].function[1]+pow(dist,2)*lights[i].function[2]),1.f);
        }
        vec3 intensity = lights[i].color;
        if(lights[i].type == 2){
            float angle = acos(dot(-intersectToSource,lights[i].dir)/(length(-intersectToSource)*length(lights[i].dir)));
            float thetaOut = lights[i].angle;
            float thetaIn = thetaOut - lights[i].penumbra;
            if(angle>thetaIn && angle<thetaOut){
                float falloff = max(0.f,min(1.f,-2.f*pow((angle-thetaIn)/(thetaOut-thetaIn),3)+3.f*pow((angle-thetaIn)/(thetaOut-thetaIn),2)));
                intensity = intensity * (1.f-falloff);
            }
            else if(angle>=thetaOut){
                intensity = vec3(0.f);
            }
        }


        float ndotl = dot(normWorld,intersectToSource);
        ndotl = max(ndotl,0.f);
        total += fatt*kd*intensity*cDiffuse*ndotl;

        vec3 reflected = normalize(reflect(-intersectToSource,normWorld));
        float reflecteddot = dot(reflected,normalize(posCam-worldPosition));
        if(reflecteddot<0.f){
            continue;
        }
        reflecteddot = shininess>0 ? pow(reflecteddot,shininess) : 1;
        total += fatt*ks*intensity*cSpecular*reflecteddot;

    }
    fragColor = vec4(clamp(total,0.0,1.0),1.f);
    fragDepth = gl_FragCoord.z;
}
