#version 330 core

in vec2 uvpass;
uniform sampler2D depthTex;
uniform sampler2D colorTex;
uniform vec2 pixelSize;
uniform float fp;
uniform float fs;
uniform float far;
uniform int dofOn;
out vec4 fragColor;

const float GOLDEN_ANGLE = 2.39996323;
const float MAX_BLUR_SIZE = 20.0;
const float RAD_SCALE = 0.5; // Smaller = nicer blur, larger = faster

float getBlurSize(float depth, float focusPoint, float focusScale)
{
        float coc = clamp((1.0 / focusPoint - 1.0 / depth)*focusScale, -1.0, 1.0);
        return abs(coc) * MAX_BLUR_SIZE;
}

vec3 depthOfField(vec2 texCoord, float focusPoint, float focusScale)
{
        float centerDepth = texture(depthTex, texCoord).r * far;
        float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
        vec3 color = texture(colorTex, texCoord).rgb;
        float tot = 1.0;
        float radius = RAD_SCALE;
        for (float ang = 0.0; radius<MAX_BLUR_SIZE; ang += GOLDEN_ANGLE)
        {
                vec2 tc = texCoord + vec2(cos(ang), sin(ang)) * pixelSize * radius;
                vec3 sampleColor = vec3(texture(colorTex, tc));
                vec4 depthVal = texture(depthTex,tc);
                float sampleDepth = depthVal.r * far;
                float sampleSize = getBlurSize(sampleDepth, focusPoint, focusScale);
                if (sampleDepth > centerDepth)
                        sampleSize = clamp(sampleSize, 0.0, centerSize*2.0);
                float m = smoothstep(radius-0.5, radius+0.5, sampleSize);
                color += mix(color/tot, sampleColor, m);
                tot += 1.0;   radius += RAD_SCALE/radius;
        }
        return color /= tot;
}



void main()
{
    if(dofOn==1){
        fragColor = vec4(depthOfField(uvpass, fp, fs),1);
    }
    else{
        fragColor = texture(colorTex, uvpass);
    }

}
