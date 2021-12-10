#version 410

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

in wireFrameVertex {
    vec4 position;
    vec2 v_texcoord;
    vec3 normal;
};
//layout (location = 0) in vec2 v_texcoord;
//layout (location = 1) in vec3 normal;
//varying vec2 v_texcoord;


uniform struct LightInfo
{
    vec4 position;  // Light position in eye coords.
    vec3 intensity;
} light;

uniform struct MaterialInfo
{
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float shininess;    // Specular shininess exponent
} material; 


void phongModel( out vec3 ambientAndDiff, out vec3 spec )
{
    // Some useful vectors
    vec3 s = normalize( vec3( light.position )/* - position.xyz*/ );
    vec3 v = normalize( -position.xyz );
    vec3 n = normalize( normal );
    vec3 r = reflect( -s, n );

    // Calculate the ambient contribution
    vec3 ambient = light.intensity * material.Ka;

    // Calculate the diffuse contribution
    float sDotN = max( dot( s, n ), 0.0 );
    vec3 diffuse = light.intensity * material.Kd * sDotN;

    // Sum the ambient and diffuse contributions
    ambientAndDiff = ambient + diffuse;

    // Calculate the specular highlight component
    spec = vec3( 0.0 );
    if ( sDotN > 0.0 )
    {
        spec = light.intensity * material.Ks *
               pow( max( dot( r, v ), 0.0 ), material.shininess );
    }
} 

//! [0]
void main()
{
    // Set fragment color from texture
    vec4 diffuseColor = texture2D(texture, v_texcoord);
    vec3 ambientAndDiff, spec;
    phongModel( ambientAndDiff, spec );
    gl_FragColor = vec4( ambientAndDiff, 1.0 ) * diffuseColor + vec4( spec, 1.0 );

}
//! [0]

