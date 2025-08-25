#version 330 core


/*vec4 gaussianBlur(sampler2D image, vec2 texCoords, bool horizontal)
{             
    float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, texCoords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 20; ++i)
        {
            result += texture(image, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 20; ++i)
        {
            result += texture(image, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    return vec4(result, 1.0);
}
*/

#ifdef GL_ES
precision mediump float;
#endif

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}


vec4 gaussianBlur(sampler2D image, vec2 texCoords)
{
	vec3 c = texture(image, texCoords.xy).rgb;

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
			final_colour += kernel[kSize+j]*kernel[kSize+i]*texture(image, (texCoords.xy+vec2(float(i),float(j)) / textureSize(image, 0).xy)).rgb;
		}
	}
		
	return vec4(final_colour/(Z*Z), 1.0);
}
