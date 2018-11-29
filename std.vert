#version 150
/* std.vert - basic transformation, no lighting */

in vec4 vertex;

out vec4 vertPosition;
out vec4 vertColor;
uniform vec3 triangleColor;

void main( void )
{
 vertColor= vec4( triangleColor, 1.0 );
 vertPosition = vertex;
}
