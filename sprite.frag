#version 150

in vec2 texcoord;

uniform sampler2D tex0;

out vec4 outColor;

void main()
{
    //outColor = texture(tex0, texcoord);
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}

