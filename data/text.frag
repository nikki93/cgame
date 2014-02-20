#version 150

in vec2 texcoord;

uniform sampler2D tex0;
uniform vec4 base_color;

out vec4 outColor;

void main()
{
    outColor = base_color * texture(tex0, texcoord);
}
