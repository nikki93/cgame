#version 150

layout(points) in;
layout(line_strip, max_vertices = 10) out;

in mat3 wmat[];
in vec2 bbmin_[];
in vec2 bbmax_[];
in float selected_[];

uniform mat3 inverse_view_matrix;

out float selected;

void main()
{
    selected = selected_[0];
    mat3 m = inverse_view_matrix * wmat[0];

    /* draw bbox */
    vec2 bbmin = bbmin_[0];
    vec2 bbmax = bbmax_[0];
    gl_Position = vec4(m * vec3(bbmin.x, bbmax.y, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmin.x, bbmin.y, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmax.x, bbmin.y, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmax.x, bbmax.y, 1.0), 1.0);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmin.x, bbmax.y, 1.0), 1.0);
    EmitVertex();
    EndPrimitive();

    /* grow bbox a little grown */
    vec4 grow = vec4(
        (m * vec3( 1, 1, 1) - m * vec3(0, 0, 1)).xy,
        (m * vec3(-1, 1, 1) - m * vec3(0, 0, 1)).xy
        );
    grow.xy = normalize(grow.xy);
    grow.zw = normalize(grow.zw);
    grow *= 0.01;
    gl_Position = vec4(m * vec3(bbmin.x, bbmax.y, 1) + vec3( grow.zw, 0), 1);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmin.x, bbmin.y, 1) + vec3(-grow.xy, 0), 1);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmax.x, bbmin.y, 1) + vec3(-grow.zw, 0), 1);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmax.x, bbmax.y, 1) + vec3( grow.xy, 0), 1);
    EmitVertex();
    gl_Position = vec4(m * vec3(bbmin.x, bbmax.y, 1) + vec3( grow.zw, 0), 1);
    EmitVertex();
    EndPrimitive();
}

