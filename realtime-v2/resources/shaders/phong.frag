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


out vec4 fragColor;

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

// shadow mapping
uniform sampler2D depthMap;
uniform mat4 lightSpaceMatrix;

// calculate shadow
float shadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    vec3 projCoords = fragPosLightSpace.xyz;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

     float bias = 0.005;
    // TODO: lightDirs[0] is the direction of the first directional light
//    float bias = max(0.05 * (1.0 - dot(vec4(normal_world, 0.f), lightDirs[0])), 0.005);
//    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // closest depth == 1, current depth < 1, for all points
//    if (closestDepth == 1.0){
//        shadow = 1.0;
//    }

    return shadow;
}

void main() {
    vec3 total = vec3(0.0,0.0,0.0);

    // shadow mapping
    vec4 position_lightspace = lightSpaceMatrix * vec4(worldPosition, 1.f);
    float shadow = shadowCalculation(position_lightspace);

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
        ndotl = min(1.f,max(ndotl,0.f));
        total += fatt*kd*intensity*cDiffuse*ndotl;

        vec3 reflected = normalize(reflect(-intersectToSource,normWorld));
        float reflecteddot = dot(reflected,normalize(posCam-worldPosition));
        if(reflecteddot<=0.f){
            continue;
        }
        reflecteddot = shininess>0 ? pow(reflecteddot,shininess) : 0;
        total += fatt*ks*intensity*cSpecular*reflecteddot;

    }

    //ambient
    vec3 ambient = ka*cAmbient;
    total += ambient;

//    total = ambient + (1.0 - shadow) * total;

    fragColor = vec4(clamp(total,0.0,1.0),1.f);

}
