#version 410

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform int RenderMode;
uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out wireFrameVertex {
    vec4 position;
    vec2 v_texcoord;
    vec3 normal;
}Out;

void main()
{
    // Calculate vertex position in screen space
    vec4 pos = vec4(a_position.x,a_position.y,a_position.z,1);
    gl_Position = mvp_matrix * model_matrix * pos;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    Out.v_texcoord = a_texcoord;
    Out.normal = a_normal;//vec3(a_texcoord.x, a_texcoord.y, 1.0);
//    Out.worldPosition = a_position;
}

