#version 410
uniform sampler2D color_texture;
in vec2 v_texcoord;
layout( location = 0 ) out vec4 FragColor;
in vec4 Position;
in vec4 PositionWorld;
uniform vec3  viewVector;
uniform vec3  PlaneNormal;
uniform vec3  PlaneOrigin;
uniform vec3  CamPos;
uniform float CamDist = 0;

vec3 intersectPlane(vec3 n, vec3 p, vec3 ro, vec3 rd)
{
    float d = dot(n, p);
    float k = -(dot(ro, n) - d ) / dot(rd, n);
    vec3 Pout = p - n*k;

    return Pout;
}

void main() {

    vec3 HitPos = intersectPlane(normalize(PlaneNormal), PlaneOrigin, CamPos, normalize(viewVector));

    vec4 texture_col = texture2D(color_texture,  v_texcoord);

    float m = 1 - length(Position.xy)*50/CamDist;
    //float m = 1 - length(PositionWorld.xyz - HitPos)/length(PlaneOrigin - HitPos);

/*    if( true  )
      FragColor = vec4(0,1,0,1);
    else
      FragColor = vec4(1,0,0,1);
*/

    vec4 final_color;
    if( texture_col.a > 0 )
      final_color = mix( texture2D(color_texture,  v_texcoord), vec4(1, 0, 0, 1), m );
    else
       final_color = texture_col;

    //if( texture_col.b == 1 && m < 0 )
     //   final_color = texture_col;

    FragColor = final_color;

    //FragColor = texture2D(color_texture,  v_texcoord);
}
