#version 150

layout(points) in;
layout(line_strip, max_vertices = 14) out;

in mat3 wmat[];
in vec2 bbmin_[];
in vec2 bbmax_[];
in float selected_[];

uniform mat3 inverse_view_matrix;
uniform float aspect;

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

    /* draw more stuff if selected */
    if (selected > 0.5)
    {
        /* screen-space coordinates */
        vec2 o = (m * vec3(0, 0, 1)).xy;       /* origin */
        vec2 u = (m * vec3(0, 1, 1)).xy - o;   /* up */
        vec2 r = (m * vec3(1, 0, 1)).xy - o;   /* right */

        /* normalize using aspect-corrected metric */
        u /= sqrt(aspect * aspect * u.x * u.x + u.y * u.y);
        r /= sqrt(aspect * aspect * r.x * r.x + r.y * r.y);

        /* draw axes */
        gl_Position = vec4(o, 0, 1);
        EmitVertex();
        gl_Position = vec4(o + 0.06 * u, 0, 1);
        EmitVertex();
        EndPrimitive();
        gl_Position = vec4(o, 0, 1);
        EmitVertex();
        gl_Position = vec4(o + 0.06 * r, 0, 1);
        EmitVertex();
        EndPrimitive();

        /* draw grown bbox */
        vec2 g1 = 0.012 * (u + r);
        vec2 g2 = 0.012 * (u - r);
        gl_Position = vec4(m * vec3(bbmin.x, bbmax.y, 1) + vec3( g2, 0), 1);
        EmitVertex();
        gl_Position = vec4(m * vec3(bbmin.x, bbmin.y, 1) + vec3(-g1, 0), 1);
        EmitVertex();
        gl_Position = vec4(m * vec3(bbmax.x, bbmin.y, 1) + vec3(-g2, 0), 1);
        EmitVertex();
        gl_Position = vec4(m * vec3(bbmax.x, bbmax.y, 1) + vec3( g1, 0), 1);
        EmitVertex();
        gl_Position = vec4(m * vec3(bbmin.x, bbmax.y, 1) + vec3( g2, 0), 1);
        EmitVertex();
        EndPrimitive();
    }
}

