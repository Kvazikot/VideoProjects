#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

in worldVertex {
    vec4 position;
    vec2 texCoords;
} In[];

out wireFrameVertex {
    noperspective vec3 edgeDistance;
    vec4 position;
    vec2 texCoords;
} Out;

uniform mat4 viewportMatrix;
attribute vec2 a_texcoord;
varying vec2 v_texcoord; 

void main()
{
    v_texcoord = a_texcoord;

    // Transform each vertex into viewport space
    vec2 p0 = vec2( viewportMatrix * ( gl_in[0].gl_Position / gl_in[0].gl_Position.w ) );
    vec2 p1 = vec2( viewportMatrix * ( gl_in[1].gl_Position / gl_in[1].gl_Position.w ) );
    vec2 p2 = vec2( viewportMatrix * ( gl_in[2].gl_Position / gl_in[2].gl_Position.w ) );

    // Calculate lengths of 3 edges of triangle
    float a = length( p1 - p2 );
    float b = length( p2 - p0 );
    float c = length( p1 - p0 );

    // Calculate internal angles using the cosine rule
    float alpha = acos( ( b * b + c * c - a * a ) / ( 2.0 * b * c ) );
    float beta = acos( ( a * a + c * c - b * b ) / ( 2.0 * a * c ) );

    // Calculate the perpendicular distance of each vertex from the opposing edge
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );

    // Now add this perpendicular distance as a per-vertex property in addition to
    // the position calculated in the vertex shader.

    // Vertex 0 (a)
    Out.edgeDistance = vec3( ha, 0, 0 );
    Out.position = In[0].position;
    Out.texCoords = In[0].texCoords;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    // Vertex 1 (b)
    Out.edgeDistance = vec3( 0, hb, 0 );
    Out.position = In[1].position;
    Out.texCoords = In[1].texCoords;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    // Vertex 2 (c)
    Out.edgeDistance = vec3( 0, 0, hc );
    Out.position = In[2].position;
    Out.texCoords = In[2].texCoords;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    // Finish the primitive off
    EndPrimitive();
}
