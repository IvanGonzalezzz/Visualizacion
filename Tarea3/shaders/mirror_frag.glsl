#version 410 core
out vec4 FragColor;

in vec4 ClipSpacePos;
uniform sampler2D samp;

void main()
{
    vec2 ndc = (ClipSpacePos.xy / ClipSpacePos.w);
    
    vec2 uv = ndc / 2.0 + 0.5;
    uv.x = 1.0 - uv.x;
    FragColor = texture(samp, uv);
}