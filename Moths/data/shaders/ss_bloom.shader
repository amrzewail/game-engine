vert
{
	#version 330 core
	layout (location = 0) in vec2 aPos;
	layout (location = 1) in vec2 aTexCoords;

	out vec2 TexCoords;

	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
		TexCoords = aTexCoords;
	}  
}

frag
{
	#version 330 core
#ifdef GL_ES
	precision mediump float;
#endif

	out vec4 FragColor;
  
	in vec2 TexCoords;

	uniform sampler2D screenTexture;


	float normpdf(in float x, in float sigma)
	{
		return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
	}


	vec4 SampleTexture(sampler2D tex, vec2 texCoords, int i, int j, float exposure)
	{
		vec4 color = texture(tex, texCoords.xy+vec2(float(i),float(j)) / textureSize(tex, 0).xy);
		float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
		if(brightness < exposure)
			color = vec4(0.0, 0.0, 0.0, 1.0);
		return color;
	}


	vec4 gaussianBlur(sampler2D image, vec2 texCoords, float exposure)
	{
		vec3 c = SampleTexture(image, texCoords.xy, 0, 0, exposure).rgb;

	//declare stuff
		const int mSize = 11;
		const int kSize = (mSize-1)/2;
		float kernel[mSize];
		vec3 final_colour = vec3(0.0);
		
		//create the 1-D kernel
		float sigma = 7.0;
		float Z = 0.0;
		for (int j = 0; j <= kSize; ++j)
		{
			kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
		}
		
		//get the normalization factor (as the gaussian has been clamped)
		for (int j = 0; j < mSize; ++j)
		{
			Z += kernel[j];
		}
		
		//read out the texels
		for (int i=-kSize; i <= kSize; ++i)
		{
			for (int j=-kSize; j <= kSize; ++j)
			{
				final_colour += kernel[kSize+j]*kernel[kSize+i]*SampleTexture(image, texCoords, i, j, exposure).rgb;
			}
		}
		
		return vec4(final_colour/(Z*Z), 1.0);
	}

	void main()
	{ 
		vec4 color = texture(screenTexture, TexCoords);
		FragColor = gaussianBlur(screenTexture, TexCoords, 1.0);

		FragColor += color;
	}
}