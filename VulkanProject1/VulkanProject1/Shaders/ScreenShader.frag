#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main()
{

	float gamma = 2.2;
	vec3 hdrColor = texture(texSampler, fragTexCoord).xyz;

	vec3 mapped = vec3(1.0) - exp(-hdrColor * 0.2);

	mapped = pow(mapped, vec3(1.0f/gamma));

	outColor = vec4(mapped, 1.0f);


	float offset = 1.0 / 300.0;

		vec2 offsets[9] = vec2[](
			vec2(-offset, offset),
			vec2(0.0f, offset),
			vec2(offset, offset),
			vec2(-offset, 0.0f),
			vec2(0.0f, 0.0f),
			vec2(offset, 0.0f),
			vec2(-offset, -offset),
			vec2(0.0f, -offset),
			vec2( offset, -offset)
		);

    /*
	//Color invers
	if(toggle==1)
	{
		FragColor = pow(vec4(vec3( 1.0f - texture(tex, TexCoord)), 1.0f), vec4(2.2));
	} 
	//GREYSCALE
	else if(toggle == 2)
	{
		FragColor = texture(tex, TexCoord);
		float avr = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 *FragColor.b;
		FragColor = vec4(avr, avr, avr, 1.0);
	}
	//SHARPEN KERNEL
	else if(toggle == 3)
	{
		float kernel[9] = float[](
			-1,	-1,	-1,
			-1,	 9,	-1,
			-1,	-1,	-1
		);
		vec3 sampleTex[9];
		for(int i=0; i<9; i++)
		{
			sampleTex[i] = vec3(texture(tex, TexCoord.st+offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i=0; i<9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0f);
	}
	//BLUR
	else if(toggle == 4)
	{
		float kernel[9] = float[](
			1.0 /16, 	2.0 / 16, 	1.0 / 16,
			2.0 /16, 	4.0 / 16, 	2.0 / 16,
			1.0 /16, 	2.0 / 16, 	1.0 / 16
		);
		vec3 sampleTex[9];
		for(int i=0; i<9; i++)
		{
			sampleTex[i] = vec3(texture(tex, TexCoord.st+offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i=0; i<9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0f);
	}
	else if(toggle == 5)
	{
		float kernel[9] = float[](
			1, 		1, 		1,
			1, 		-8, 	1,
			1, 		1, 		1

		);
		vec3 sampleTex[9];
		for(int i=0; i<9; i++)
		{
			sampleTex[i] = vec3(texture(tex, TexCoord.st+offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i=0; i<9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0f);
	}
	//REGULAR
	else
	{
		float exposure = 1.0;
		// float depthValue = texture(tex, TexCoord).r;
    	//  FragColor = vec4(vec3(depthValue), 1.0);
		vec3 hdrColor = texture(tex, TexCoord).rgb;
		vec3 bloomColor = texture(blur, TexCoord).rgb;
		vec3 skyCol = texture(sky,TexCoord).rgb;
		hdrColor += skyCol;
		hdrColor += bloomColor;
		vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

		FragColor = vec4(mapped , 1.0);
		//FragColor = texture(tex, TexCoord);
	}
	*/
	//outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
}


/*
 	-1.0f,  1.0f, -1.0f,	//0
    -1.0f, -1.0f, -1.0f,	//1
     1.0f, -1.0f, -1.0f,	//2
     1.0f, -1.0f, -1.0f,	//2
     1.0f,  1.0f, -1.0f,	//3
    -1.0f,  1.0f, -1.0f,	//0

    -1.0f, -1.0f,  1.0f,	//4
    -1.0f, -1.0f, -1.0f,	//1
    -1.0f,  1.0f, -1.0f,	//0
    -1.0f,  1.0f, -1.0f,	//0
    -1.0f,  1.0f,  1.0f,	//5
    -1.0f, -1.0f,  1.0f,	//4

     1.0f, -1.0f, -1.0f,	//2
     1.0f, -1.0f,  1.0f,	//6
     1.0f,  1.0f,  1.0f,	//7
     1.0f,  1.0f,  1.0f,	//7
     1.0f,  1.0f, -1.0f,	//3
     1.0f, -1.0f, -1.0f,	//2

    -1.0f, -1.0f,  1.0f,	//4
    -1.0f,  1.0f,  1.0f,	//5
     1.0f,  1.0f,  1.0f,	//7
     1.0f,  1.0f,  1.0f,	//7
     1.0f, -1.0f,  1.0f,	//6
    -1.0f, -1.0f,  1.0f,	//4

    -1.0f,  1.0f, -1.0f,	//0
     1.0f,  1.0f, -1.0f,	//3
     1.0f,  1.0f,  1.0f,	//7
     1.0f,  1.0f,  1.0f,	//7
    -1.0f,  1.0f,  1.0f,	//5
    -1.0f,  1.0f, -1.0f,	//0

    -1.0f, -1.0f, -1.0f,	//1
    -1.0f, -1.0f,  1.0f,	//4
     1.0f, -1.0f, -1.0f,	//2
     1.0f, -1.0f, -1.0f,	//2
    -1.0f, -1.0f,  1.0f,	//4
     1.0f, -1.0f,  1.0f		//6

	 */