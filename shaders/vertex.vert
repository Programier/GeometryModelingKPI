#version 310 es
precision highp float;

layout(location = 0) out vec3 out_color;

uniform vec2 point1;
uniform vec2 point2;
uniform vec3 color1;
uniform vec3 color2;


vec2 get_point()
{
    if (gl_VertexID == 0)
    {
        return point1;
    }
    else
    {
        return point2;
    }
}

vec3 get_color()
{
    if (gl_VertexID == 0)
    {
        return color1;
    }
    else
    {
        return color2;
    }
}

void main(void)
{
    out_color   = get_color();
    gl_Position = vec4(get_point().xy, 0.0, 1.0);
}
