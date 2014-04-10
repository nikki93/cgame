#version 150

in vec2 texcoord;
in float is_cursor;

uniform sampler2D tex0;
uniform vec4 base_color;
uniform float cursor_blink;

out vec4 outColor;

void main()
{
    if (is_cursor > 0)
        outColor = vec4(base_color.xyz, cursor_blink);
    else
        outColor = base_color * texture(tex0, texcoord);
}
