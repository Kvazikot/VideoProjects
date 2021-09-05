//==========================
// oaoiaoiia iiea
//==========================
uniform float HBlurSampler; //0,00000;0,00000;10,00000
uniform float ClearDepth; //0,00000;0,00000;10,00000
uniform float Saturation; //0,00000;0,00000;10,00000
uniform float Tiles; //2,39996;0,00000;10,00000
uniform float Speed; //0,00000;0,00000;0,00000
#extension GL_ARB_texture_rectangle : enable
varying vec2 texture_coordinate; 
varying vec2 texture_coordinate2;
uniform sampler2D my_color_texture;

// Uniform variables for texturing
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

// CCIR 601 standard
vec3 std601R =  vec3(  1.164 ,  0    ,  1.4022   );
vec3 std601G =  vec3(  1.164 , -0.391, -0.813    );
vec3 std601B =  vec3(  1.164 ,  2.018,  0        );
//float[4] stdbias =  float[4]( -0.0625, -0.5  , -0.5, 0   );
// CCIR 709 standard (Garry amann)vec3 std709R = vec3(  1.1644,  0.0008,  1.7932  );
vec3 std709G = vec3(  1.1642, -0.2131,  0.5328   );
vec3 std709B = vec3(  1.1665,  2.1124,  0.0011   );
vec4 stdbias = vec4( -0.0625, -0.5  , -0.5, 0  );
// This does YUY2 to RGB conversion
vec4 yuyv_texture_sampler(	in sampler2D texture,
							in vec2 texcoord)
{
	vec4 outColor;
	vec4 texColor0;
	vec4 texColor1;
	vec2 tc0, tc1;
	float isOddUV, texel_sample, texel_offset;
	
       //fract(1.);
	isOddUV = floor(fract( texcoord.x * 0.5) * 2.);
	texel_sample = 1.0;
	texel_offset = +0.5;

	//tc0.x = floor(texcoord.x) - (	1 * texel_sample) + texel_offset;
	//tc1.x = tc0.x + texel_sample;
	//tc0.y = texcoord.y;
	//tc1.y = texcoord.y;

	texColor0 = texture2D(texture, texcoord);
	texColor1 = texture2D(texture, texcoord);
	
	// For L8A8, assume A8<-alpha L8<-rgb
	texColor0.r = texColor0.r; // assign Y0 (1st position) automatic
	texColor0.g = texColor0.a; // assign U0 (2nd position)
	texColor0.b = texColor1.a; // assign V0 (3rd position)

	texColor1.r = texColor1.r; // assign Y1 (1st position) automatic
	texColor1.g = texColor0.a; // assign U0 (2nd position)
	texColor1.b = texColor1.a; // assign V0 (3rd position)
	
	// assume RGBA0 (Y0 U0)
	// assume RGBA1 (Y1 V0)
	//texColor1 = texColor1 * vec4(isOddUV);

	// assume RGBA0 (Y0 U0)
	// assume RGBA1 (Y1 V0)
	//texColor0 = texColor0 * vec4(1.-isOddUV);

	//texColor0 = texColor0 + texColor1 + stdbias;
	
    outColor = vec4(dot(std601R, texColor0.rgb),
					dot(std601G, texColor0.rgb),
					dot(std601B, texColor0.rgb),
					1.0 );
	return outColor;
}


float line_filter()
{
   vec4 CC;
   vec2 PP;
   vec2 ox = vec2(0.001,0.0);
   vec2 oy = vec2(0.0,0.001);
   
   PP = texture_coordinate - oy;
   CC = texture2D(tex0,PP - ox); 
   vec4 g00 = CC;
   CC = texture2D(tex0,PP); 
   vec4 g01 = CC;
   CC = texture2D(tex0,PP + ox); 
   vec4 g02 = CC;

   PP = texture_coordinate;
   CC = texture2D(tex0,PP - ox); 
   vec4 g10 = CC;
   CC = texture2D(tex0,PP); 
   vec4 g11 = CC;
   CC = texture2D(tex0,PP + ox); 
   vec4 g12 = CC;

   PP = texture_coordinate + oy;
   CC = texture2D(my_color_texture,PP - ox); 
   vec4 g20 = CC;
   CC = texture2D(my_color_texture,PP); 
   vec4 g21 = CC;
   CC = texture2D(my_color_texture,PP + ox); 
   vec4 g22 = CC;


   float K00 = -1.;

	float K01 = -2.;
	float K02 = -1.;
	float K10 = 0.;
	float K11 = -2.;
	float K12 = 0.;
	float K20 = 1.;
	float K21 = 4.;
	float K22 = 1.;
	vec4 sx = vec4(0,0,0,0);
	vec4 sy = vec4(0,0,0,0);
	sx += g00 * K00;
	sx += g01 * K01;
	sx += g02 * K02;
	sx += g10 * K10;
	sx += g11 * K11;
	sx += g12 * K12;
	sx += g20 * K20;
	sx += g21 * K21;
	sx += g22 * K22; 
	sy += g00 * K00;
	sy += g01 * K10;
	sy += g02 * K20;
	sy += g10 * K01;
	sy += g11 * K11;
	sy += g12 * K21;
	sy += g20 * K02;
	sy += g21 * K12;
	sy += g22 * K22; 

	float dist = sqrt(sx.x*sx.x+sy.x*sy.x);
	float result = 1.;
	if (dist>Tiles) { result = 0.; }
      return result;
}

vec4 erode()
{
   vec4 CC;
   vec2 PP;
   vec2 ox = vec2(Tiles,0.0);
   vec2 oy = vec2(0.0,0.0001);
   
   PP = texture_coordinate - oy;
   CC = texture2D(tex1,PP - ox); 
   vec4 g00 = CC;
   CC = texture2D(tex1,PP); 
   vec4 g01 = CC;
   CC = texture2D(tex1,PP + ox); 
   vec4 g02 = CC;

   PP = texture_coordinate;
   CC = texture2D(tex1,PP - ox); 
   vec4 g10 = CC;
   CC = texture2D(tex1,PP); 
   vec4 g11 = CC;
   CC = texture2D(tex1,PP + ox); 
   vec4 g12 = CC;

   PP = texture_coordinate + oy;
   CC = texture2D(my_color_texture,PP - ox); 
   vec4 g20 = CC;
   CC = texture2D(my_color_texture,PP); 
   vec4 g21 = CC;
   CC = texture2D(my_color_texture,PP + ox); 
   vec4 g22 = CC;

   vec4 tmp, tmp_min, tmp_max;
   
   tmp.r = min(g00.r, g01.r);
   tmp.r = min(tmp.r, g02.r);
   tmp.r = min(tmp.r, g10.r);
   tmp.r = min(tmp.r, g12.r);
   tmp.r = min(tmp.r, g20.r);
   tmp.r = min(tmp.r, g21.r);
   tmp.r = min(tmp.r, g22.r);
   tmp.b = tmp.r;
   tmp.g = tmp.r;
   tmp_min = tmp;

   tmp.r = max(g00.r, g01.r);
   tmp.r = max(tmp.r, g02.r);
   tmp.r = max(tmp.r, g10.r);
   tmp.r = max(tmp.r, g12.r);
   tmp.r = max(tmp.r, g20.r);
   tmp.r = max(tmp.r, g21.r);
   tmp.r = max(tmp.r, g22.r);
   tmp.b = tmp.r;
   tmp.g = tmp.r*tmp.r*tmp.r;
   tmp_max = tmp;

   return tmp;
}


// assume "t" ranges from 0 to 1 safely
// brute-force this, it's running on the CPU
vec3 c_bezier(in vec3 c0, in vec3 c1, in vec3 c2, in vec3 c3, in float t)
{
	float t2 = t*t;
	float t3 = t2*t;
	float nt = 1.0 - t;
	float nt2 = nt*nt;
	float nt3 = nt2 * nt;
	vec3 b = nt3*c0 + (3.0*t*nt2)*c1 + (3.0*t2*nt)*c2 + t3*c3;
	return b;
}

// function used to fill the volume noise texture
vec4 color_curve(in vec2 Pos) 
{
    vec3 color0 = vec3(-0.1,0.0,0.3);
    vec3 color1 = vec3(2.1,0.3,-0.1);
    vec3 color2 = vec3(1.5,2.4,0.0);
    vec3 color3 = vec3(0.1,0.3,1.1);
    vec3 sp = c_bezier(color0,color1,color2,color3,Pos.x);
    return vec4(sp,1);
}

const vec3 GrayConv = vec3(0.25, 0.65, 0.10);

vec4 gryGradPS(in sampler2D texture, in vec2 texcoord)
{   
	vec4 scnColor;
	//if (UseYUV) {
	//	scnColor = yuyv_texture_sampler(texture, texcoord.xy);
	//} else {
		scnColor = texture2D(texture, texcoord.xy);
	//}
	float  n = dot(scnColor.xyz,GrayConv);
	return vec4(color_curve(vec2(n,0)).xyz,scnColor.w);
} 


float[11] samples = float[11]( 0.0222244,0.0378346,0.0755906,0.1309775,0.1756663, 0.1974126, 0.1756663,0.1309775,0.0755906,0.0378346,0.0222244 ); 
//float samples[11];
// = { 
// 0.0222244,0.0378346,0.0755906,0.1309775,0.1756663, 0.1974126, 0.1756663,0.1309775,0.0755906,0.0378346,0.0222244 
//}; 
vec4 sum;
  

bool end_rec = false;
void getj(int i)
{
  int j;
  
  for( j= -5 ;j < 5; j++)
  {
    sum += texture2D(tex0, texture_coordinate+ vec2(j, i)*0.0008)* samples[j+5];

  }
}

void main(void)
{   
  int i;
  sum = vec4(0,0,0,0);
  for( i= -5 ;i < 5; i++)
  {
    getj(i);
  }
  
  //gl_FragColor = sum*sum*0.010 + texture2D(tex0, texture_coordinate); 

//   vec2 PP = texture_coordinate;
//    gl_FragColor = vec4((abs(dFdx(tc))) ;
//   vec4 CC;
   vec4 CC, CC1;
   float a = line_filter();
   CC = vec4(a,a,a,1);   
   CC = gryGradPS(tex0, texture_coordinate);   
//CC = erode(); 
   //CC = texture2D(tex0, texture_coordinate);
      
   //2-ie i?ioia 
   if(CC.r == 255.)
   {
     // CC.r = 2;
      //CC.b = 255;
   } 
 

   //1-ue i?ioia
   if(CC.r == 0.)
   { 
       //CC.r = 255;
   }
   
//   if(texture_coordinate.x > 0.5)      
//    { 
//       CC1 = texture2D(tex0, texture_coordinate); 
//       CC = CC1; 
//   } 

   //CC = yuyv_texture_sampler(tex0, texture_coordinate);
  
  gl_FragColor = sum*sum*0.01 + CC;
 }
