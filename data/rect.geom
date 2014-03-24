#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in mat3 wmat[];
in vec2 size_[];

uniform mat3 inverse_view_matrix;

void main()
{
    mat3 m = inverse_view_matrix * wmat[0];

    gl_Position = vec4(m * vec3(0, -size_[0].y, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(0, 0, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(size_[0].x, -size_[0].y, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(size_[0].x, 0, 1.0), 1.0);
    EmitVertex();
    EndPrimitive();
}

