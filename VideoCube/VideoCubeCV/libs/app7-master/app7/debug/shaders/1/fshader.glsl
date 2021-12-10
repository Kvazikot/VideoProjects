#version 410

uniform sampler2D texture;

//layout (location = 1) in vec2 v_texcoord;

layout (location = 0) out vec4 fragColor;

in wireFrameVertex {
    vec4 position;
    vec2 texCoords;
    vec3 edgeDistance;
}In;

//! [0]
void main()
{
    // Set fragment color from texture
    fragColor = texture2D(texture, In.texCoords);
}
//! [0]

