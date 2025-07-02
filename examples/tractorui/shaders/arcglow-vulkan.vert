#version 440

layout( location = 0 ) in vec4 in_vertex;
layout( location = 1 ) in vec2 in_coord;
layout( location = 0 ) out vec2 coord;

void main()
{
    coord = in_coord;
    gl_Position = in_vertex;
} 