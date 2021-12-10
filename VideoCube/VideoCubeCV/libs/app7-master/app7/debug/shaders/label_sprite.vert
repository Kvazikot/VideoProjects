#version 410
out vec2 v_texcoord;
in vec3 position;
in vec2 a_texcoord;
out vec4 Position;
out vec4 PositionWorld;
uniform mat4 mvp;
uniform mat4 InvProjection;
uniform mat4 InvView;
uniform mat4 texMat;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    PositionWorld = InvProjection * InvView * vec4(position, 1.0);
    Position = gl_Position;
    vec4 tc = texMat * vec4(a_texcoord.x, a_texcoord.y, 0, 1);
    v_texcoord = tc.xy;
}
