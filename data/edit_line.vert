#version 150

in vec2 position;
in float point_size;
in vec4 color;

out vec4 color_;

uniform mat3 inverse_view_matrix;

void main()
{
    gl_Position = vec4(inverse_view_matrix * vec3(position, 1.0), 1.0);
    gl_PointSize = point_size;
    color_ = color;
}
