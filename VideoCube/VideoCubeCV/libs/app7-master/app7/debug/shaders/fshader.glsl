#version 410

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;

in wireFrameVertex {
    vec4 position;
    vec2 v_texcoord;
    vec3 normal;
//    vec3 worldPosition;
};

//Режим рендеринга
uniform int RenderMode;
const int ColorRendering  = 0;
const int MultiMaterial  = 1;
const int SingleTexture   = 2;
const int Phong = 3;

uniform vec3 eyePosWorld;
uniform vec4 COLOR;
uniform vec2 ranges[50];
uniform int map_flags[50];
uniform vec4 diffuse_colors[50];
uniform int n_ranges;
uniform int stopSignal;


uniform struct LightInfo
{
    vec4 position;  // Light position in eye coords.
    vec3 intensity;
} light;

uniform struct MaterialInfo
{
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float shininess;    // Specular shininess exponent
} material; 


vec4 Lighten(vec4 col, float amount)
{
    if(col.r > col.g && col.r > col.b)
    {
        col.r+=amount;
    }
    if(col.r > 1) col.r = 1;
    return col;
}

void phongModel( out vec3 ambientAndDiff, out vec3 spec )
{
    // Some useful vectors
    vec3 viewDir = normalize(eyePosWorld.xyz - position.xyz); 
    vec3 s = normalize( vec3( light.position ) + viewDir /* - position.xyz*/ );
    vec3 v = normalize( -position.xyz );
    vec3 n = normalize( normal );
    vec3 r = reflect( -s, n );

    // Calculate the ambient contribution
    vec3 ambient = light.intensity * material.Ka;

    // Calculate the diffuse contribution
    float sDotN = max( dot( s, n ), 0.0 );
    vec3 diffuse = light.intensity * material.Kd * sDotN;

    // Sum the ambient and diffuse contributions
    ambientAndDiff = ambient + diffuse;

    // Calculate the specular highlight component
    spec = vec3( 0.0 );
    if ( sDotN > 0.0 )
    {
        spec = light.intensity * material.Ks *
               pow( max( dot( r, v ), 0.0 ), material.shininess );
    }
} 

vec4 chooseTexture(int i)
{
//   if(i%2==0) return vec4(1,0,0,1);
//   else return vec4(0,1,0,1);
   vec4 color;
   switch(i)
   {
        case 0: color = texture2D(texture0, v_texcoord);
	break;
        case 1: color = texture2D(texture1, v_texcoord);
	break;
        case 2: color = texture2D(texture2, v_texcoord);
	break;
        case 3: color = texture2D(texture3, v_texcoord);
	break;
        case 4: color = texture2D(texture4, v_texcoord);
	break;
        case 5: color = texture2D(texture5, v_texcoord);
	break;
        case 6: color = texture2D(texture6, v_texcoord);
	break;
        case 7: color = texture2D(texture7, v_texcoord);
	break;
	default:
	   color = vec4(1,1,0,1);
 	break;
   }
   return color;

}

vec4 chooseCol(int i)
{
  return diffuse_colors[i];
}

//! [0]
void main()
{
    // Set fragment color from texture
    vec4 diffuseColor;
    vec4 diffuseColor2;
    uint RenderModeL = 2;
    switch(RenderMode)
    {
	case ColorRendering:
           gl_FragColor = COLOR;
        break;

       case SingleTexture:
	    gl_FragColor = texture2D(texture, v_texcoord);
       break;
       
       case MultiMaterial:
       {

	    int id = gl_PrimitiveID;	
	    for(int i=0; i<n_ranges; i++)
	    {
		if( id > ranges[i].x && id < ranges[i].y)
		{
		    if( map_flags[i] == 0 )
	   	       diffuseColor2 = chooseCol(i);
		    else
	               diffuseColor2 = chooseTexture(i);
		    if( stopSignal == 1 && i == 0 )
	               diffuseColor2 = Lighten(diffuseColor2, 0.5);//mix(vec4(,0,0,1), diffuseColor2, 0.7);
	//	    diffuseColor2 = mix(diffuseColor, diffuseColor2, 0.5);
		}
	    }
	    vec3 ambientAndDiff, spec;
	    phongModel( ambientAndDiff, spec );    
	    gl_FragColor = vec4( ambientAndDiff, 1.0 ) * diffuseColor2 + vec4( spec, 1.0 );
	}
	break;
        case Phong:
	    diffuseColor = texture2D(texture, v_texcoord);
	    vec3 ambientAndDiff, spec;
	    phongModel( ambientAndDiff, spec );    
	    gl_FragColor = vec4( ambientAndDiff, 1.0 ) * diffuseColor + vec4( spec, 1.0 );	
	break;
	default:
	   gl_FragColor = vec4(1,1,0,1);
 	break;
    }



}
//! [0]

