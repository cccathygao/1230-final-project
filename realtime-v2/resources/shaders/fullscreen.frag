#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D depthMap;     // location 1
uniform sampler2D colorBuffer;  // location 0

void main()
{
    fragColor = vec4(1.f);

    // set fragColor with depth
    float depthValue = texture(depthMap, uv).r;
    fragColor = vec4(vec3(depthValue), 1.0);

    // debug

    if (depthValue == 1.0){
        fragColor = vec4(0.5);
    }

    // currently all depthValue = 1.0

    // set fragColor using the sampler2D at the UV coordinate
//    fragColor = texture(colorBuffer, uv);
}
