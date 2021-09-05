//==========================
// шахматное поле
//==========================
uniform float HBlurSampler; //0,00000;0,00000;10,00000
uniform float ClearDepth; //0,00000;0,00000;10,00000
uniform float Saturation; //0,00000;0,00000;10,00000
uniform float Tiles; //0,14623;0,00000;10,00000
uniform float Speed; //0,00000;0,00000;0,00000
#extension GL_ARB_texture_rectangle : enable
varying vec2 texture_coordinate; 
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

float line_filter()
{
   vec4 CC;
   vec2 PP;
   vec2 ox = vec2(0.001,0.0);
   vec2 oy = vec2(0.0,0.001);
   
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


   float K00 = -1;

	float K01 = -2;
	float K02 = -1;
	float K10 = 0;
	float K11 = -2;
	float K12 = 0;
	float K20 = 1;
	float K21 = 4;
	float K22 = 1;
	float sx = 0;
	float sy = 0;
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

	float dist = sqrt(sx*sx+sy*sy);
	float result = 1;
	if (dist>Tiles) { result = 0; }
      return result;
}

float erode()
{
   vec4 CC;
   vec2 PP;
   vec2 ox = vec2(0.001,0.0);
   vec2 oy = vec2(0.0,0.001);
   
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

   float result = 1;
   if (dist>Tiles) { result = 0; }
   return result;
}


void main(void)
{   
   float l = erode();   
   gl_FragColor = vec4(l, l ,l, 1.0 ); 


   //c.x = Tiles;
  // gl_FragColor = c;  
   //if(texture_coordinate.x > 0.1 )
    // gl_FragColor = texture2D(my_color_texture, texture_coordinate);   
// Sampling The Texture And Passing It To The Frame Buffer
  
   //gl_FragColor.xyz = gl_TexCoord[0].xyz;//   
}
