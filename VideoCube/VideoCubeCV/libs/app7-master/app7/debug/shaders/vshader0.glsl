#version 410

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

varying vec2 v_texcoord;
out wireFrameVertex {
    vec4 position;
    vec2 v_texcoord;
    vec3 normal;
}Out;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;
    Out.position = gl_Position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    Out.v_texcoord = a_texcoord;
    Out.normal = a_normal;//vec3(a_texcoord.x, a_texcoord.y, 1.0);
}

