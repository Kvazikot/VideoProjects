uniform vec3  BrickColor, MortarColor;
uniform vec2  BrickSize;
uniform vec2  BrickPct;
uniform vec2  MortarPct;

varying vec2  MCposition;
varying float LightIntensity;

bool pulse = 0;

#define Integral(x, p, notp) ((floor(x)*(p)) + max(fract(x)-(notp), 0.0))

void main(void)
{
    vec2 position, fw, useBrick;
	vec3 color;
    
    
    // Determine position within the brick pattern
    position = MCposition / BrickSize;

	/*if(MCposition.y > BrickSize.y)
	{
	  position.x = BrickSize.x;
	  position.y = BrickSize.y;
	  MCposition.x = BrickSize.x;
	  MCposition.y = BrickSize.y;
	  //gl_FragColor = vec4 (color, 1.0);
	}
	*/
	  
    // Adjust every other row by an offset of half a brick
    if (fract(position.y * 0.5) > 0.5)
    {
        position.x += 0.3 ;
    }

    // Calculate filter size
    fw = fwidth(position) + 0.3; //fwidth not implemented on WildcatVP
    fw = (abs(dFdx(MCposition))) ;
 	
    // Perform filtering by integrating the 2D pulse made by the
    // brick pattern over the filter width and height
    useBrick = (Integral(position + fw, BrickPct, MortarPct) - 
                Integral(position, BrickPct, MortarPct)) / fw;

    // Determine final color
    //color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    //color *= LightIntensity;
    //if(fract(MCposition.y / 3) == 0 )
    
    //определяем колво клеток
    int size = 10;
    if((fract(MCposition.x / size) == 0) && (fract(MCposition.y / size) != 0))
      pulse = !pulse;
    
    //сделаем шахматное поле
    if(pulse == true )
      color = vec3(1,1,1);
    else
      color = vec3(0,0,0);
    //color = vec3((float)(pulse / 14),0,0);
    
    gl_FragColor = vec4 (color, 1.0);
}
