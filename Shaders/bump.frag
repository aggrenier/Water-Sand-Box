
varying vec3 N, L;
varying vec2 texture_coordinate; 

uniform sampler2D tex1, tex2, tex3;

void main() 
{ 
	const float specularExp = 128.0;
	vec3 NN = normalize(N);
	
	vec2 coord = vec2(1.0-texture_coordinate.x, 1.0-texture_coordinate.y);
	vec3 NMAP = texture2D(tex2, coord).xyz;
	NMAP = NMAP -0.5;
	NN = normalize(NN + NMAP);
	
	vec4 dColor = texture2D(tex1, coord);
	
	gl_FragColor = vec4(0.0);
	
	vec3 NL = normalize(L);
	vec3 NH = normalize(NL + vec3(0.0, 0.0, 1.0));
	float NdotL = max(0.0, dot(NN, NL));
    
    
	float t3 = texture2D(tex3, coord).x;

	gl_FragColor.rgb += dColor.rgb * NdotL;

	if (NdotL > 0.0)
		gl_FragColor.rgb += pow(max(0.0, dot(NN, NH)), specularExp) * t3;
	
	gl_FragColor.a = 1.0;
}