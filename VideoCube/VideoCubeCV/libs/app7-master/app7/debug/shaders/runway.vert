#version 410
in vec2 a_texcoord;
out gl_PerVertex {
    vec4 gl_Position;
};
out vec2 v_texcoord;
in vec3 position;
uniform mat4 mvp;
uniform mat4 texMat;
in vec3 color;

void main() {
    vec4 tmp;
    vec4 out_pos;
    out_pos = mvp * vec4(position,1.0);
    gl_Position = out_pos;
    tmp = vec4(a_texcoord, 1.0, 0.0) * texMat;
    v_texcoord = tmp.xy;
}
/*
in vec4 Position;
out vec3 vPosition;

void main()
{
    vPosition = Position.xyz;
}

*/
