#version 330 core

// declare "in" variables for the world-space position and normal,
// received post-interpolation from the vertex shader
in vec3 position_world;
in vec3 normal_world;
in vec2 uv;

// declare an out vec4 for your output color
out vec4 fragColor;

uniform float k_a;
uniform float k_d;
uniform float k_s;
uniform float shininess;

uniform int numLights;
uniform int lightTypes[8];          // 0: directional, 1: point, 2: spot
uniform vec4 lightColors[8];
uniform vec4 lightDirs[8];
uniform vec4 lightPositions[8];
uniform vec3 lightFunctions[8];     // attenuation function
uniform float lightPenumbras[8];    // spot light only
uniform float lightAngles[8];       // spot light only

uniform vec4 camera_pos;

uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform float blend;

// texture mapping
uniform bool texture_map_on;
uniform sampler2D textureImg;
uniform float repeatU;
uniform float repeatV;

// shadow mapping
uniform sampler2D depthMap;
uniform mat4 lightSpaceMatrix;

// debug only
float depth(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r;

    return closestDepth;
}

// calculate shadow
float shadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
//    float bias = 0.005;
    // TODO: lightDirs[0] is the direction of the first directional light
    float bias = max(0.05 * (1.0 - dot(vec4(normal_world, 0.f), lightDirs[0])), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

// falloff function for spot light
float falloff(float angle, float penumbra, float x){
    float inner = angle - penumbra;
    float r = (x - inner) / (angle - inner);
    return -2.f * pow(r, 3) + 3.f * pow(r, 2);
}

void main() {
    // Remember that you need to renormalize vectors here if you want them to be normalized

//    fragColor = vec4(1.f);
//    return;

    // shadow mapping
    vec4 position_lightspace = lightSpaceMatrix * vec4(position_world, 1.f);
    float shadow = shadowCalculation(position_lightspace);


    // texture mapping
    vec4 textureBlend;
    if (texture_map_on){
        vec2 curr_uv = vec2(uv[0] * repeatU, uv[1] * repeatV);
        vec4 textureColor = texture(textureImg, curr_uv);
        textureBlend = blend * textureColor + (1 - blend) * k_d * cDiffuse;
    } else{
        textureBlend = k_d * cDiffuse;
    }

    fragColor = vec4(0.f);

    for (int j = 0; j < numLights; j++){
        // 0: directional light
        if (lightTypes[j] == 0){
            // add the diffuse term
            vec4 pos_to_light = normalize(-lightDirs[j]);
            fragColor += lightColors[j] * textureBlend * max(dot(normalize(vec4(normal_world, 0.f)), normalize(pos_to_light)), 0.f);

            // add the specular term
            vec4 reflected_dir = reflect(-pos_to_light, normalize(vec4(normal_world, 0.f)));
            vec4 directionToCamera = camera_pos - vec4(position_world, 1.f);
            float temp = 1.f;
            if (shininess > 0){
                temp = pow(max(dot(normalize(reflected_dir), normalize(directionToCamera)), 0.f), shininess);
            }
            fragColor += lightColors[j] * k_s * cSpecular * temp;
        }

        // 1: point light
        else if (lightTypes[j] == 1){
            vec4 pos_to_light = normalize(lightPositions[j] - vec4(position_world, 1.f));
            float dist = length(lightPositions[j] - vec4(position_world, 1.f));
            float f_att = min(1.f / (lightFunctions[j][0] + dist * lightFunctions[j][1] + dist * dist * lightFunctions[j][2]), 1.f);

            // add the diffuse term
            fragColor += textureBlend * f_att * lightColors[j] * max(dot(normalize(vec4(normal_world, 0.f)), normalize(pos_to_light)), 0.f);

            // add the specular term
            vec4 reflected_dir = reflect(-pos_to_light, normalize(vec4(normal_world, 0.f)));
            vec4 directionToCamera = camera_pos - vec4(position_world, 1.f);
            float temp = 1.f;
            if (shininess > 0){
                temp = pow(max(dot(normalize(reflected_dir), normalize(directionToCamera)), 0.f), shininess);
            }
            fragColor += f_att * lightColors[j] * k_s * cSpecular * temp;
        }

        // 2: spot light
        else {
            // calculate the angle between light.dir and (intersect_pos - light.pos)
            float x = acos(dot(normalize(lightDirs[j]), normalize(vec4(position_world, 1.f) - lightPositions[j])));

            if (x > lightAngles[j]){
                continue;
            }

            vec4 pos_to_light = normalize(lightPositions[j] - vec4(position_world, 1.f));
            float dist = length(lightPositions[j] - vec4(position_world, 1.f));
            float f_att = min(1.f / (lightFunctions[j][0] + dist * lightFunctions[j][1] + dist * dist * lightFunctions[j][2]), 1.f);

            // add the diffuse term
            vec4 temp_illumination = textureBlend * f_att * lightColors[j] * max(dot(normalize(vec4(normal_world, 0.f)), normalize(pos_to_light)), 0.f);
            // add the specular term
            vec4 reflected_dir = reflect(-pos_to_light, normalize(vec4(normal_world, 0.f)));
            vec4 directionToCamera = camera_pos - vec4(position_world, 1.f);
            float temp = 1.f;
            if (shininess > 0){
                temp = pow(max(dot(normalize(reflected_dir), normalize(directionToCamera)), 0.f), shininess);
            }
            temp_illumination += f_att * lightColors[j] * k_s * cSpecular * temp;

            float inner = lightAngles[j] - lightPenumbras[j];
            if (x > inner && x <= lightAngles[j]){
                temp_illumination *= 1 - falloff(lightAngles[j], lightPenumbras[j], x);
            }
            fragColor += temp_illumination;
        }
    }

    // add the ambient term
    vec4 ambient = vec4(k_a) * cAmbient;

//    fragColor += ambient;
    fragColor = ambient + (1.0 - shadow) * fragColor;
//    fragColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
}
