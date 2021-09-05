 uniform sampler2D texture;
 uniform float alpha;
 uniform float lum;
 
 void main()
 {
   vec4 color = texture2D(texture,gl_TexCoord[0].st);
   //color = mix(lum,color,alpha);   
   gl_FragColor = lum * (1.0 - alpha) + color;
 }
