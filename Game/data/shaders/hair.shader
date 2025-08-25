#define CULL_FACE_OFF
//#define TRANSPARENT
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
	uniform vec4 ucolor;

	out vec4 FragColor;

	float hash(vec2 input)
	{
		return fract(1.0e4 * sin(17.0 * input.x + 0.1 * input.y) * (0.1 + abs(sin(13.0 * input.y + input.x))));
	}

	float hash3D(vec3 input)
	{
		return hash(vec2(hash(input.xy), input.z));
	}

	float HashedAlpha(float hashScale)
	{
		
		float maxDeriv = max(length(dFdx(TexCoord.xy)), length(dFdy(TexCoord.xy)));
		float pixScale = 1.0 / (hashScale * maxDeriv);
		vec2 pixScales = vec2(exp2(floor(log2(pixScale))), exp2(ceil(log2(pixScale))));
		vec2 alpha = vec2(hash(floor(pixScales.x * TexCoord.xy)), hash(floor(pixScales.y * TexCoord.xy)));
		//vec2 alpha = vec2(hash3D(floor(pixScales.x * Position.xyz)), hash3D(floor(pixScales.y * Position.xyz)));
		float lerpFactor = fract(log2(pixScale));
		float x = (1 - lerpFactor) * alpha.x + lerpFactor * alpha.y;
		float a = min(lerpFactor, 1 - lerpFactor);
		vec3 cases = vec3(x*x/(2*a*(1-a)), (x-0.5*a) / (1-a), 1.0 - ((1-x)*(1-x) / (2*a*(1-a))));
		float aT = (x < (1-a)) ? ((x<a) ? cases.x : cases.y) : cases.z;
		aT = clamp(aT, 1.0e-6, 1.0);

		return aT;
	}
	
	void main() 
	{
		vec4 texColor = texture(tex0, TexCoord);

		if(texColor.r < HashedAlpha(1)) discard;

		vec4 color = vec4(1,1,1,1);

		//color = APPLY_DIRECTIONAL_LIGHT(color);

		vec3 normal = normalize(Normal);

		vec3 lightDir = normalize(-directionalLight.direction);

		float light = max(dot(normal, lightDir), 0);
		light = map(light, 0, 1, 0.2, 1);

		color *= light;

		color += vec4(ambientColor.rgb, 0);

		color *= ucolor;

		//color = max(color + ucolor * 0.25, color);

		FragColor = color;

		return;
				

		//color = APPLY_AMBIENT(color, texColor);

		//vec3 camPosition = CameraPosition();

		//vec3 normal = normalize(Normal);

		//vec3 viewDirection = CameraDirection();
		//float camDirValue = dot(CameraDirection(), normalize(Position - camPosition));
		//viewDirection *= map01(step(0.8, camDirValue) * camDirValue, 0.8, 1);

		//float rim = pow(1 - saturate(dot(viewDirection, normal)), 4);

		////color -= color * rim * 0.5;

		//float light = max(dot(normal, directionalLight.direction), -0.5);
		//light = map(light, -0.5, 1, 0, 1);

		//float backlight = saturate(dot(viewDirection, -directionalLight.direction));

		//color += backlight * texColor * directionalLight.color / 2;

		//color += backlight * light * texColor / 2;

		//color += backlight * light * rim * texColor * directionalLight.color / 2;

		//float backlightAlpha = backlight * light;

		//color.a *= (1 - saturate(backlightAlpha / 2));

		//color *= ucolor;

		//FragColor = color;
	}
}