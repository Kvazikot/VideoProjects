#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture; // inital conditions
uniform sampler2D texture0; // render texture
uniform int fbo_pass;
uniform int n_frame;
uniform int RES_X;
uniform int RES_Y;
uniform vec4 color;
varying vec2 v_texcoord;

//! [0]
void main()
{

    // Set fragment color from texture
   //if( fract(float(n_frame) * 0.5) == 0 )
   gl_FragColor = texture2D(texture, v_texcoord);
   if( fbo_pass == 1 )
   {
     //if( v_texcoord.x > 0.2 )
    //   v_texcoord.x = 1 - v_texcoord.x;

       // правила клеточного автомата
       vec2 N[8];
       vec2 o = v_texcoord;
       float dx = 1. / float(RES_X);
       float dy = 1. / float(RES_Y);
       N[0] = vec2(o.x-dx, o.y-dy);
       N[1] = vec2(o.x, o.y-dy);
       N[2] = vec2(o.x+dx, o.y-dy);
       N[3] = vec2(o.x-dx, o.y);
       N[4] = vec2(o.x+dx, o.y);
       N[5] = vec2(o.x-dx, o.y+dy);
       N[6] = vec2(o.x, o.y+dy);
       N[7] = vec2(o.x+dx, o.y+dy);
       int n_live_neibours = 0;
       for(int i=0; i < 8; i++ )
       {
           vec4 c = texture2D(texture, N[i]);
           if( c.r < 0.1 )
             n_live_neibours++;
       }
       vec4 c = texture2D(texture, v_texcoord);
       if( (c.r < 0.1) && (n_live_neibours < 2) ) // underpopulation
       {
         gl_FragColor = vec4(1,1,1,1);
       }
       if( (c.r < 0.1) && (n_live_neibours > 3) ) // overpopulation
         gl_FragColor = vec4(1,1,1,1);
       if( (c.r > 0.9) && (n_live_neibours == 3) ) //reproduction.
         gl_FragColor = vec4(0,0,0,0);
   }




//    if( texture2D(texture, v_texcoord).r < 0.1 )
//        if((v_texcoord.x * 10) < n_frame)
//          gl_FragColor = color;





}
//! [0]

