/*uniform mat4 matWorldViewInverseTranspose;
uniform mat4 matWorldView;
uniform mat4 matView;
uniform mat4 matWorldViewProjection;*/

attribute vec3 tangent; 
attribute vec3 binormal;

varying vec2 texCoord;
varying vec3 eyeSpaceVert;
varying vec3 eyeSpaceTangent;
varying vec3 eyeSpaceBinormal;
varying vec3 eyeSpaceNormal;
varying vec3 eyeSpaceLight;

void main(void)
{ 
   eyeSpaceVert = (gl_ModelViewMatrix * gl_Vertex).xyz;
   eyeSpaceLight = (gl_ModelViewMatrix * vec4(gl_LightSource[0].position.xyz,1.0)).xyz;
  //eyeSpaceLight = vec4(gl_LightSource[0].position.xyz,1.0).xyz;
                       
   eyeSpaceTangent  = gl_NormalMatrix * tangent;
   eyeSpaceBinormal = gl_NormalMatrix * binormal;
   eyeSpaceNormal   = gl_NormalMatrix * gl_Normal;
   
   texCoord   = gl_MultiTexCoord0.xy;
   
   gl_Position = ftransform();
}
