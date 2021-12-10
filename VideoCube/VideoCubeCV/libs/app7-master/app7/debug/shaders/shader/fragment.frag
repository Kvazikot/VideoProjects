#version 410

uniform float LineWidth;
uniform vec4 LineColor;
uniform vec4 LightPosition;
uniform vec3 LightIntensity;
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess = 0.3;
uniform float CamDist = 0;
uniform vec3  CamPos;
uniform vec4  PPMColor;
noperspective in vec3 EdgeDistance;
in vec3 Normal;
in vec3 MyNormal;
in vec4 Position;

layout( location = 0 ) out vec4 FragColor;

vec4 phongModel(vec3 pos, vec3 norm ) {
    vec3 s = normalize(vec3(LightPosition) - pos);

    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = LightIntensity * Ka;

    float sDotN = max( dot(s,norm), 0.0 );

    vec3 diffuse = LightIntensity * Kd * sDotN;

    vec3 spec = vec3(0.0);

    if( sDotN > 0.0 )
        spec = LightIntensity * Ks *
               pow( max( dot(r,v), 0.0 ), shininess );

    return vec4(ambient + diffuse + spec, 1);
}

float edgeMix()
{
    // Find the smallest distance
    float d = min( min( EdgeDistance.x, EdgeDistance.y ), EdgeDistance.z );

    if( d < LineWidth - 1 ) {
        return 1.0;
    } else if( d > LineWidth + 1 ) {
        return 0.0;
    } else {
        float x = d - (LineWidth - 1);
        return exp2(-2.0 * (x*x));
    }
}

float intersectPlane(vec3 n, vec3 p, vec3 ro, vec3 rd)
{
    float d = dot(n, p);
    float k = (d - dot(ro, n)) / dot(rd, n);
    vec3 Pout = p - n*k;
    return Pout;
}

void main()
{
    //vec3 Position = vec3(TEPosition.x,TEPosition.y,TEPosition.z);
    vec4 lightColor = phongModel(Position.xyz, Normal);
    float a = 2*abs(atan(Normal.y/Normal.x))/3.14 ;
    //float a = 2*abs(atan(MyNormal.y/MyNormal.x)/3.14);
    float mixVal = edgeMix();
    //lightColor = pow( lightColor, vec4(1.0/2.2) );

    float m = 1 - length(Position.xy)*50/CamDist;

    //-- Пропадает с приближением расстояния
    lightColor = vec4(m+PPMColor.r,PPMColor.g,PPMColor.b,CamDist/150);

    vec4 cyl_color = mix( lightColor, LineColor, mixVal );
    //FragColor = vec4( 0.5 * ( Normal + vec3( 1.0 ) ), 1.0 );
    FragColor = cyl_color;
    //FragColor = vec4(beam*100,0,0,1);

}
