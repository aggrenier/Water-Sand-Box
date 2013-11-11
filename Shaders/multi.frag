
varying vec2 texture_coordinate; 
varying vec2 map_coordinate;

uniform sampler2D tex1, tex2, tex3;

void main() 
{ 
	vec4 t1 = texture2D(tex1, texture_coordinate);
	vec4 t2 = texture2D(tex2, texture_coordinate);
	vec4 t3 = texture2D(tex3, map_coordinate);

	gl_FragColor.rgb = (t3.rgb * t1.rgb) + (t3.g * t2.rgb);
	
	gl_FragColor.a = 1.0;
}