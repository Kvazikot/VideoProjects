// Uniform variables for texturing
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;

varying vec2 texture_coordinate;
varying vec2 texture_coordinate2;

  
void main(void)
{
// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
// Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
	texture_coordinate = vec2(gl_MultiTexCoord0);
      texture_coordinate2 = vec2(gl_MultiTexCoord1);
}