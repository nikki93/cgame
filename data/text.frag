#version 150

in vec2 texcoord;
in float is_cursor;

uniform sampler2D tex0;
uniform vec4 base_color;

out vec4 outColor;

void main()
{
    if (is_cursor > 0)
        outColor = base_color;
    else
        outColor = base_color * texture(tex0, texcoord);
}
