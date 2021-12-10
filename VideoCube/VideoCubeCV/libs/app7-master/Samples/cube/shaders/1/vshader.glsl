#version 410

uniform mat4 mvp_matrix;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texcoord;

out terrainVertex {
    vec4 position;
    vec2 v_texcoord;
} Out;
 

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;
    Out.position = gl_Position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    Out.v_texcoord = a_texcoord;
}
