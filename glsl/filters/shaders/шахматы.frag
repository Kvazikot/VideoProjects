//==========================
// Мой первый щейдер
// шахматное поле
//==========================
uniform float Msize;
varying vec2  MCposition;
bool pulse = 0;
float d = 0.0;


void main(void)
{
    vec2 position, fw, useBrick;
	vec3 color;	
	float x,y;
	
	x = MCposition.x;
	y = MCposition.y;

    // крест top
  	if ((y> 0.2) && (y < 0.9))	
	  if((x > -0.1) && (x < 0.1))
	  	pulse = true;
	  else
	    pulse = false;

	if ((y > 0.5) && (y < 0.7))	
	  if((x > -0.3) && (x < 0.3))
	  	pulse = true;
	  else
	    pulse = false;

    // крест right
  	if ((y> -0.4) && (y < 0.2))	
	  if((x > 0.5) && (x < 0.7))
	  	pulse = true;
	  //else
	  //  pulse = false;

	if ((y > -0.2) && (y < 0))	
	  if((x > 0.2) && (x < 1))
	  	pulse = true;
	    
    // крест left
  	if ((y> -0.4) && (y < 0.2))	
	  if((x > -0.7) && (x < -0.5))
	  	pulse = true;

	if ((y > -0.2) && (y < 0))	
	  if((x > -1) && (x < -0.3))
	  	pulse = true;

	    

     //круг
     float R = 0.3;
     if ((y > -0.4) && (y < 0.2))
       if((x > -0.3) && (x < 0.3))	
       {
       	  if((x*x + (y+0.1)*(y+0.1)) < R * R)
       	    pulse=true;
       	  else
       	    pulse=false;
       }

	// подножие
	 if ((y > -0.9) && (y < -0.37))
	 {
	 	if ((x > -0.5) && (x < -0.1))
	 	{
       		  if(y < (3.75*(x+0.5)*(x+0.5) - 0.9))
       	    	pulse=true;
       	  	else
	       	    pulse=false;	 	
		 }	 	 
	 	if ((x > 0.1) && (x < 0.5))
	 	{
       	  	if(y < (3.75*(x-0.5)*(x-0.5) - 0.9))
	       	    pulse=true;
       	  	else
	       	    pulse=false;	 	
	 	}
	 	if ((x > -0.1) && (x < 0.1))
	 	  pulse=true;
	 }

    //сделаем шахматное поле
/*    if((fract(MCposition.x * Msize  ) > 0.5))
      pulse = !pulse;
    if((fract(MCposition.y  * Msize ) > 0.5))
      pulse = !pulse;
*/      

    if(pulse == true )
    {
      color = vec3(0,0,0);
      d = 0;      
    }
    else
    {
      //d = smoothstep(0.1,0.2,fract(MCposition.x * Msize  ));
      d = 1 ;
      color = vec3(d,d,d);            
    }
 	//fw = (abs(dFdx(MCposition))) ;
 	color.x = MCposition.x;
    //color = vec3((float)(pulse / 14),0,0);
    
    
    gl_FragColor = vec4 (color, 1.0);
}
