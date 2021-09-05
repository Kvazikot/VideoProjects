varying vec2  p;

void main()
{
	p = gl_Vertex.xy;
    //gl_TexCoord[0] = gl_MultiTexCoord0;
  //  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_Position     = ftransform();

} 
