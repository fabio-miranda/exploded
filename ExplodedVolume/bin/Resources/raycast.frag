//#extension GL_ARB_shader_texture_lod : require


uniform sampler2D backSampler;
//uniform sampler2D frontSampler;
uniform sampler3D volumeSampler;
varying vec2 texCoord;
varying vec4 pos;
varying vec4 posInterpolated;


void main()
{
	//vec2 posicao1 = vec2(gl_FragCoord.x / 1280.0, gl_FragCoord.y / 720.0);
	//same as the above. Get the screen position of the fragment
	vec2 fragCoord = (((pos.xy / pos.w) + 1.0) / 2.0);


	vec3 start = posInterpolated.xyz;
	
	vec3 final = texture2D(backSampler, fragCoord).xyz; //sample the back face
    

    vec3 direction = normalize(final - start);
	
	
	float stepSize = 0.5;
	vec3 step = direction * stepSize;
	
	vec4 color;
	vec4 acc;
	
	for(int i=0; i<10; i++){
		
		color = texture3D(volumeSampler, final);

		

		acc.rgb = mix(acc.rgb, color.rgb, color.a);
		acc.a = mix(color.a, 1.0, acc.a);



		start += step;
		
		if(start.x > 1.0 || start.y > 1.0 || start.z > 1.0)
			break;
		
		

	}
	
	
    

	gl_FragColor = acc + vec4(1, 0, 0, 1);
	//gl_FragColor = vec4(final, 1);
	//gl_FragColor = vec4(0.5, 0, 0, 1);
	//gl_FragColor = vec4(backFaceCoord.x, backFaceCoord.y, 0, 1);
}

