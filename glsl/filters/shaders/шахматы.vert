//
uniform vec3 LightPosition;

const float SpecularContribution = 0.3;
const float DiffuseContribution  = 1.0 - SpecularContribution;

varying float LightIntensity;
varying vec2  MCposition;

float waveTime = 0;
uniform float waveWidth;
uniform float waveHeight;
  
void main(void)
{
    vec3 ecPosition = vec3 (gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);
    float diffuse   = max(dot(lightVec, tnorm), 0.0);
    float spec      = 0.0;

  	vec4 v = vec4(gl_Vertex);
  	int i;
  	
  	/*waveTime+=11;
  	
  	for(i=0; i<10000; i++)
  	{
      	v.z = sin(sqrt(v.x));  
  	}
        
  	v.z = sin(waveWidth * v.x + waveTime) * cos(waveWidth * v.y + waveTime) * waveHeight;
        
  	gl_Position = gl_ModelViewProjectionMatrix * v; 

    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }*/
    MCposition      = gl_Vertex.xy;
    

    LightIntensity  = DiffuseContribution * diffuse +
                      SpecularContribution * spec;
	
	//ecPosition.x = gl_Vertex.z;
	//gl_Vertex.x = gl_Vertex.y;
	gl_Vertex.z = gl_Vertex.z + noise4(gl_Vertex.x);
	
    MCposition      = gl_Vertex.xy;
    gl_Position     = ftransform();
    
}
