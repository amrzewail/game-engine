#define CULL_FACE_OFF
#define TRANSPARENT
#define Blend_SrcAlpha_OneMinusSrcAlpha

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

		vec4 color = APPLY_DIRECTIONAL_LIGHT(texColor);

		color = APPLY_AMBIENT(color, texColor);

		FragColor = color;
	}
}