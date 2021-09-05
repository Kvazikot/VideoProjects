1//==========================
2// шахматное поле
3//==========================
4uniform float HBlurSampler; //0,00000;0,00000;10,00000
5uniform float ClearDepth; //0,00000;0,00000;10,00000
6uniform float Saturation; //0,00000;0,00000;10,00000
7uniform float Tiles; //0,00000;0,00000;10,00000
8#extension GL_ARB_texture_rectangle : enable
9#define half4 vec4
10#define half2 vec2
11#define half float
12
13// CCIR 601 standard
14vec3 std601R =  vec3(  1.164 ,  0    ,  1.4022   );
15
16vec3 std601G =  vec3(  1.164 , -0.391, -0.813    );
17vec3 std601B =  vec3(  1.164 ,  2.018,  0        );
//float[4] stdbias =  float[4]( -0.0625, -0.5  , -0.5, 0   );

// CCIR 709 standard (Garry amann)vec3 std709R = vec3(  1.1644,  0.0008,  1.7932  );
vec3 std709G = vec3(  1.1642, -0.2131,  0.5328   );
vec3 std709B = vec3(  1.1665,  2.1124,  0.0011   );
vec4 stdbias = vec4( -0.0625, -0.5  , -0.5, 0  );

// Uniform variables for texturinguniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;

uniform float TexSize; //53,62119;0,00000;100,00000
uniform float Contrast; //0,00000;0,00000;10,00000
varying vec2  MCposition;

bool pulse = 0;
float d = 0.0;// This does YUY2 to RGB conversion

vec4 yuyv_texture_sampler(	in sampler2DRect texture, in vec3 texcoord)
{	
    vec4 outColor;	
    vec3 texColor0;	
    vec3 texColor1;	
    vec2 tc0, tc1;	
    float isOddUV, texel_sample, texel_offset;		

    isOddUV = floor(frac(texcoord.x * 0.5) * 2);	
    texel_sample = 1.0;	
    texel_offset = +0.5;	
    tc0.x = floor(texcoord.x) - (isOddUV * texel_sample) + texel_offset;	tc1.x = tc0.x + texel_sample;	
    tc0.y = texcoord.y;	
    tc1.y = texcoord.y;	
    texColor0 = texture2DRect(texture, tc0);	
    texColor1 = texture2DRect(texture, tc1);		// For L8A8, assume A8<-alpha L8<-rgb	
    texColor0.r = texColor0.r; 
    // assign Y0 (1st position) automatic	
    texColor0.g = texColor0.b; 
    // assign U0 (2nd position)	
    texColor0.b = texColor1.y; 
    // assign V0 (3rd position)	
    texColor1.r = texColor1.x; 
    // assign Y1 (1st position) automatic	
    texColor1.g = texColor0.y; 
    // assign U0 (2nd position)	
    texColor1.b = texColor1.y; 
    // assign V0 (3rd position)		// assume RGBA0 (Y0 U0)	// assume RGBA1 (Y1 V0)	texColor1 = texColor1 * vec3(isOddUV);	// assume RGBA0 (Y0 U0)	// assume RGBA1 (Y1 V0)	
    texColor0 = texColor0 * vec3(1-isOddUV);	
    texColor0 = texColor0 + texColor1 + stdbias;	   
    outColor = vec4(dot(std601R, texColor0),dot(std601G, texColor0), dot(std601B, texColor0),	1.0 );
    return outColor;
}
	
void main(void)
{   
    vec2 position, fw, useBrick;	vec3 color;		
    float x,y;		

    //сделаем шахматное поле   
    if((fract(MCposition.x * Tiles) > 0.5))     
    pulse = !pulse;       

    if((fract(MCposition.y  * Tiles) > 0.5))     
    pulse = !pulse;         

    if(pulse == true )   
    {      
        color = vec3(1,0,1);     
        d = 0;         
    }    
    else   
    {     
        //d = smoothstep(0.1,0.2,fract(MCposition.x * TexSize  ));  
        d = 1 ; 
	  color = vec3(d,d,d);               
    } 
    //fw = (abs(dFdx(MCposition))) ; 	
   // color.x = MCposition.x;   
    color = vec3((float)(pulse / 14),0,0);  
    gl_FragColor = yuyv_texture_sampler(tex0, gl_TexCoord[0]);   
    gl_FragColor.xyz = gl_TexCoord[0].xyz;//   
    //color.xyz = texture2DRect(tex0, gl_TexCoord[0].xy).xyz;  
    //gl_FragColor = vec4 (color,1.0);   
    //gl_FragColor = gl_FragColor + vec4 (color, 1.0);
}
