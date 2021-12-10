#version 410
uniform sampler2D color_texture;
in vec2 v_texcoord;
layout(location=0) out vec4 fragColor;
out vec4 color;
void main() {
    fragColor = texture2D(color_texture,  v_texcoord); //color = vec4(fragColor, 1.0);
}
