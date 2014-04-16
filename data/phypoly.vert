#version 150

in vec2 position;

uniform mat3 inverse_view_matrix;
uniform mat3 wmat;

void main()
{
    vec2 scale = vec2(length(wmat * vec3(1, 0, 1) - wmat * vec3(0, 0, 1)),
                      length(wmat * vec3(0, 1, 1) - wmat * vec3(0, 0, 1)));
    gl_Position = vec4(inverse_view_matrix * wmat * vec3(position / scale, 1.0), 1.0);
}
