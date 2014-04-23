#version 150

in vec2 position;

uniform mat3 inverse_view_matrix;
uniform mat3 wmat;
uniform float radius;
uniform vec2 offset;

void main()
{
    vec2 scale = vec2(length(wmat * vec3(1, 0, 1) - wmat * vec3(0, 0, 1)),
                      length(wmat * vec3(0, 1, 1) - wmat * vec3(0, 0, 1)));
    vec2 localpos = radius * position / scale + offset;
    gl_Position = vec4(inverse_view_matrix * wmat * vec3(localpos, 1.0), 1.0);
    gl_PointSize = 5;
}
