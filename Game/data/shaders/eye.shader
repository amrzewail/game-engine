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
	#include "data/shaders/noise.frag"

	uniform sampler2D tex0;

	out vec4 FragColor;


	void main() 
	{
		vec3 normal = normalize(Normal);

		vec4 texColor = texture(tex0, TexCoord);

		vec4 color = texColor;

		float light = MainLight();

		vec3 camPos = CameraPosition();

		vec4 eyeHighlight = directionalLight.color + color;
		float eyeHighlightT = 0;
		eyeHighlightT = pow(light, 30);
		eyeHighlightT = step(0.3, eyeHighlightT) * eyeHighlightT * 2;

		vec3 viewDirection = normalize(camPos - Position);
		vec4 eyeRim = color * vec4(20,0, 0, 1);
		float eyeRimT = pow(1 - saturate(dot(viewDirection, normal)), 4);
		
		color = mix(color, eyeHighlight, eyeHighlightT);

		color = mix(color, eyeRim, eyeRimT);

		FragColor = color;
	
		//FragColor = length(camPos - Position) * vec4(1,1,1,1);
	}
}