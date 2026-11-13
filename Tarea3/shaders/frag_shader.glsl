#version 410 core

in vec2 tc;
uniform sampler2D samp;

out vec4 color;


void main()
{
    color = texture(samp,tc);
}