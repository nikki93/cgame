#version 150

in float selected;

out vec4 color;

void main()
{
    if (selected > 0.5)
        color = vec4(1, 0, 0, 1);
    else
        color = vec4(0, 0.7, 0, 1);
}
