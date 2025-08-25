//#define Blend_SrcAlpha_OneMinusSrcAlpha

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
		vec4 color = vec4(1,1,1,1);
		vec4 texColor = texture(tex0, TexCoord);

		color = texColor;

		color = APPLY_TINT(color);
		
		color = APPLY_DIRECTIONAL_LIGHT(color);

		color = APPLY_AMBIENT(color, texColor);

		color = APPLY_FOG(color);

		FragColor = color;
	}
}