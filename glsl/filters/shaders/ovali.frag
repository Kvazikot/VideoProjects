uniform float d;
uniform float f;
//параметрические переменные
uniform float a,b,c;
vec3 col, col2;
bool pul = true;
bool pul2 = true;
varying vec2  p;
uniform sampler2D texture; 

// x  -1...1
// y  -1...1

//перевод из полярных координат в декаровые
vec3 polar2dec(float alfa, float r)
{
    vec3 output;
    output.x = r*cos(alfa);
    output.y = r*sin(alfa);
    return output;
}

//овалы кассини
void oval()
{
	float _t1, _t2;
	float cond; 
	p.x = p.x * 10;
	p.y = p.y * 10;
	cond = d * d * d * d - f * f * f * f;
	_t1 = p.x * p.x + p.y * p.y;
	_t2 = p.x * p.x - p.y * p.y;
	
	if(((_t1 * _t1 - 2 * d * d * _t2) - cond) > -10.11)
	 pul = false;
	else
	 pul = true;
}

vec3 col_tab[3] = {vec3(0.4,0.1,0.77),vec3(0.6,0.7,1),vec3(0.33,1,1)};
varying int idx = 0;
float R=6;

//окружность концентрическая
void circ()
{
	float _t1, cc,dc, freq;
	float cond; 
	int sw;
	int col_kr;
	
	p.x = p.x * 10;
	p.y = p.y * 10;
	
	//работаем в [-0.1,0.1]
	if(p.x < -R*2)
	  return;
	if(p.x > R*2)
	  return;
	
	_t1 = p.x * p.x + p.y * p.y;
	
	//циклим
	//col_kr = floor(p.x*100)	;	
	sw = 0; cc = 0;
	col_kr = a;
	dc = 1/col_kr; //delta color 
	for(int i=0; i<col_kr; i++)
	{
	  if( _t1 > (R*i/col_kr ) && _t1 < (R - R*i/col_kr))
	  {
	    cc += dc; 
	    col = vec3(1 - cc,cc,1 - cc);	    
	    col = vec3(1 - sw,sw,0+sw);
	   }
	    //col = vec3(1 - i/10,1,0 + i/10);
	   sw = !sw;
	}
	
	if( _t1 > 0.1 && _t1 < 0.2)
	{
		//col = vec3(1,0,0);
	 }
	else
	{
	 }
}

void main()
{
	//вход - p позиция точки на текстуре
	int N = 10;
	
	vec4 color = texture2D(texture,gl_TexCoord[0].st);  
	
	//верт решетка
	if( fract(p.x * N) < 0.3)
	 pul2 = true;
	else
	 pul2 = false;
	
	//овал кассини
	//oval();
	 
	 circ();
	 
	/*if(pul) 
	 col = vec3(1,1,1);
	else
	  col = vec3(0,0,0);
	*/
	
	  
	if(pul2) 
	 col2 = vec3(1,0,0);
	else
	  col2 = vec3(0,0,0);
	  
	col = mix(col, color, 0.5);

	 //выход 
	 gl_FragColor = vec4(col, 1.0);
	
}
