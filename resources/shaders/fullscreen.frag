#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D depthMap;     // location 1
uniform sampler2D colorBuffer;  // location 0
uniform bool invert_filter_on;
uniform bool grayscale_filter_on;
uniform bool blur_filter_on;
uniform bool sharpen_filter_on;
uniform int width;
uniform int height;



const float blur_kernel[25] = float[](1.f/25, 1.f/25, 1.f/25, 1.f/25, 1.f/25,
                                        1.f/25, 1.f/25, 1.f/25, 1.f/25, 1.f/25,
                                        1.f/25, 1.f/25, 1.f/25, 1.f/25, 1.f/25,
                                        1.f/25, 1.f/25, 1.f/25, 1.f/25, 1.f/25,
                                        1.f/25, 1.f/25, 1.f/25, 1.f/25, 1.f/25);

const float sharpen_kernel[9] = float[](-1.f/9, -1.f/9, -1.f/9,
                                         -1.f/9, 17.f/9, -1.f/9,
                                         -1.f/9, -1.f/9, -1.f/9);

vec4 convolve2D(float u, float v, bool blur_filter) {
    vec4 finalColor = vec4(0.f);

    int kernel_sidelen = 0;

    if (blur_filter){
        kernel_sidelen = 5;
    } else{
        kernel_sidelen = 3;
    }

    int offset = - kernel_sidelen / 2;

    for (int i = offset; i < offset+kernel_sidelen; i++){
        for (int j = offset; j < offset+kernel_sidelen; j++){
            float curr_u = u + j * 1.f / width;
            float curr_v = v + i * 1.f / height;
            vec4 currColor = texture(colorBuffer, vec2(curr_u, curr_v));

            int kernel_ind = (kernel_sidelen - 1 - (i - offset)) * kernel_sidelen + (kernel_sidelen - 1 - (j - offset));
            float kernel_val = 0;
            if (blur_filter){
                kernel_val = blur_kernel[kernel_ind];
            } else{
                kernel_val = sharpen_kernel[kernel_ind];
            }

            finalColor += currColor * kernel_val;
        }
    }

    finalColor = vec4(finalColor.xyz, 1.f);
    return finalColor;
}

void main()
{
    fragColor = vec4(1.f);

//    // set fragColor with depth
//    float depthValue = texture(depthMap, uv).r;
//    fragColor = vec4(vec3(depthValue), 1.0);

//    // set fragColor using the sampler2D at the UV coordinate
//    fragColor = texture(colorBuffer, uv);

//    // invert fragColor's r, g, and b color channels
//    if (invert_filter_on){
//        fragColor = vec4(vec3(1.f) - fragColor.xyz, 1.f);
//    }

//    // grayscale
//    if (grayscale_filter_on){
//        fragColor = vec4(vec3(fragColor.r * 0.299 + fragColor.g * 0.587 + fragColor.b * 0.114), 1.f);
//    }

//    // box
//    if (blur_filter_on){
//        fragColor = convolve2D(uv[0], uv[1], true);
//    }

//    // sharpen
//    if (sharpen_filter_on){
//        fragColor = convolve2D(uv[0], uv[1], false);
//    }
}
