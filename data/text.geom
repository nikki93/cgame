#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 pos_[];
in vec2 cell_[];

out vec2 texcoord;

uniform vec2 base_pos;
uniform vec2 size;
uniform vec2 inv_grid_size;

void main()
{
    vec2 offset = base_pos + size * pos_[0];

    gl_Position = vec4(offset + vec2(   0.0, size.y), 0.0, 1.0);
    texcoord = inv_grid_size * (cell_[0] + vec2(0.0, 1.0));
    EmitVertex();

    gl_Position = vec4(offset + vec2(   0.0,    0.0), 0.0, 1.0);
    texcoord = inv_grid_size * (cell_[0] + vec2(0.0, 0.0));
    EmitVertex();

    gl_Position = vec4(offset + vec2(size.x, size.y), 0.0, 1.0);
    texcoord = inv_grid_size * (cell_[0] + vec2(1.0, 1.0));
    EmitVertex();

    gl_Position = vec4(offset + vec2(size.x,    0.0), 0.0, 1.0);
    texcoord = inv_grid_size * (cell_[0] + vec2(1.0, 0.0));
    EmitVertex();

    EndPrimitive();
}

