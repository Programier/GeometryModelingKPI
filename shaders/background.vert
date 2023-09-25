#version 310 es
precision highp float;

vec2 positions[6] = vec2[](vec2(-1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0),
                           vec2(-1.0, -1.0), vec2(1.0, 1.0), vec2(1.0, -1.0));

uniform vec3 ld_color;
uniform vec3 lu_color;
uniform vec3 rd_color;
uniform vec3 ru_color;

layout(location = 0) out vec3 out_color;

vec3 get_color()
{
    vec3 colors[6] = vec3[](ld_color, lu_color, ru_color, ld_color, ru_color, rd_color);
    return colors[gl_VertexID];
}

void main()
{
    gl_Position = vec4(positions[gl_VertexID].xy, 0.0, 1.0);
    out_color = get_color();
}
