#version 150

layout(points) in;
layout(line_strip, max_vertices = 5) out;

in mat3 wmat[];
in vec2 bbmin_[];
in vec2 bbmax_[];
in float selected_[];

uniform mat3 inverse_view_matrix;

void main()
{
    mat3 m = inverse_view_matrix * wmat[0];

    gl_Position = vec4(m * vec3(bbmin_[0].x, bbmax_[0].y, 1.0), 1.0);
    EmitVertex();

    gl_Position = vec4(m * vec3(bbmin_[0].x, bbmin_[0].y, 1.0), 1.0);
    EmitVertex();

    gl_Position = vec4(m * vec3(bbmax_[0].x, bbmin_[0].y, 1.0), 1.0);
    EmitVertex();

    gl_Position = vec4(m * vec3(bbmax_[0].x, bbmax_[0].y, 1.0), 1.0);
    EmitVertex();

    gl_Position = vec4(m * vec3(bbmin_[0].x, bbmax_[0].y, 1.0), 1.0);
    EmitVertex();

    EndPrimitive();
}

