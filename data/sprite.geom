#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in mat3 transform[];
in vec2 cell_[];
in vec2 size_[];

out vec2 texcoord;

uniform mat3 inverse_view_matrix;

uniform vec2 atlas_size;

void main()
{
    mat3 m = inverse_view_matrix * transform[0];

    gl_Position = vec4(m * vec3(-0.5, 0.5, 1.0), 1.0);
    texcoord = (cell_[0] + size_[0] * vec2(0.0, 1.0)) / atlas_size;
    EmitVertex();

    gl_Position = vec4(m * vec3(-0.5, -0.5, 1.0), 1.0);
    texcoord = (cell_[0] + size_[0] * vec2(0.0, 0.0)) / atlas_size;
    EmitVertex();

    gl_Position = vec4(m * vec3(0.5, 0.5, 1.0), 1.0);
    texcoord = (cell_[0] + size_[0] * vec2(1.0, 1.0)) / atlas_size;
    EmitVertex();

    gl_Position = vec4(m * vec3(0.5, -0.5, 1.0), 1.0);
    texcoord = (cell_[0] + size_[0] * vec2(1.0, 0.0)) / atlas_size;
    EmitVertex();

    EndPrimitive();
}

