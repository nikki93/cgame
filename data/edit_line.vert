#version 150

in vec2 position;

uniform mat3 inverse_view_matrix;

void main()
{
    gl_Position = vec4(inverse_view_matrix * vec3(position, 1.0), 1.0);
}
