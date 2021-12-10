#version 440 core

in vec3 tePosition;

layout(triangles) in;
layout(triangles, max_vertices = 4) out;

void main()
{
    gl_Position = vec4(tePosition, 1.0);
}
