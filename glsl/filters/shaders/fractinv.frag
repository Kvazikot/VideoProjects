// Fragment shader for drawing Julia sets
//
// Authors: Dave Baldwin, Steve Koren, Randi Rost
//          based on a shader by Michael Rivero
//
// Copyright (c) 2002-2004 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//

varying vec3  Position;
uniform float MaxIterations;
uniform int TIME_FROM_INIT;
uniform float AnimSpeed;
uniform float AnimLenght;
uniform float Xcenter;
uniform float Ycenter;
uniform vec3  InnerColor;
uniform vec3  OuterColor1;
uniform vec3  OuterColor2;
uniform float Creal;
uniform float Cimag;
uniform float Zoom;

void main(void)
{

    float real  = Position.x * Zoom + Xcenter;
    float imag  = Position.y * Zoom + Ycenter;        
    //float   Creal = real;   // Change this line...
    //float   Cimag = imag;   // ...and this one to get a Julia set
    
    float   Creal = real /((real*real)+(imag*imag));   // Change this line...
    float   Cimag = (-imag)/((real*real)+(imag*imag));   // ...and this one to get a Julia set
    
     real = Creal;
     imag = Cimag;

    float r2 = 0.0;
    float iter;
    
	//float c2 = 0.0;

    for (iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
    {
    			
        float tempreal = real;

        real = (tempreal * tempreal) - (imag * imag) + (Creal);
  		imag = 2.0 * tempreal * imag + (Cimag);
    
        r2   = (real * real) + (imag * imag);
    }

    // Base the color on the number of iterations

    vec3 color;

    if (r2 < 4.0)
        color = InnerColor;
    else
        color = mix(OuterColor1, OuterColor2, fract(iter * 0.05));

    //gl_FragColor = vec4 (clamp(color, 0.0, 1.0), 1.0);
    gl_FragColor = vec4 (color, 1.0);
}
