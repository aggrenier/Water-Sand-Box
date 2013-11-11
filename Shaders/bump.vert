
uniform vec3 lightPos;
varying vec3 N, L;
varying vec2 texture_coordinate;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	N = gl_NormalMatrix * gl_Normal;
	
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	L = lightPos - V.xyz;

	gl_FrontColor = gl_Color;
	texture_coordinate = vec2(gl_MultiTexCoord0);
}