
uniform vec2 wallSize;
uniform vec3 offset;

varying vec2 texture_coordinate;
varying vec2 map_coordinate;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_FrontColor = gl_Color;
	texture_coordinate = vec2(gl_MultiTexCoord0);
	map_coordinate.x = ((gl_Vertex.xyz + offset).x / wallSize.x) + 0.5;
	map_coordinate.y = (gl_Vertex.xyz + offset).y / wallSize.y;
}