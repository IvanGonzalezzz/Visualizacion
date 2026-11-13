#version 410 core

in vec2 tc;
uniform sampler2D samp;

out vec4 color;


void main()
{
    vec3 objectColor = texture(samp,tc).rgb;
    color = vec4(objectColor,1.0);
}