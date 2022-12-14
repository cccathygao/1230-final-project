 #version 330 core

in vec2 uvpass;

uniform sampler2D sampler;

out vec4 fragColor;

void main(void)
{

    fragColor = clamp(texture(sampler,vec2(uvpass[0],uvpass[1])),0.f,1.f);
}
