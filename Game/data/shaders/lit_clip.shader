#define CULL_FACE_OFF

vert
{
	#version 330 core

	#include "data/shaders/core.vert"

	void main() 
	{
		gl_Position = APPLY_TRANSFORMATION();
	}
}

frag
{
	#version 330 core

	#include "data/shaders/core.frag"

	uniform sampler2D tex0;

	out vec4 FragColor;
	
	void main() 
	{
		vec4 texColor = texture(tex0, TexCoord);

		if(length(texColor.rgb) < 0.2 || texColor.a < 0.2)
		{
			discard;
		}

		vec4 color = APPLY_DIRECTIONAL_LIGHT(texColor);

		color = APPLY_AMBIENT(color, texColor);

		FragColor = color;
	}
}