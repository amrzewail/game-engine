struct DirectionalLight
{
	vec3 direction;
	vec4 color;
	float intensity;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
flat in ivec4 BoneIds;
in vec4 BoneWeights;

uniform DirectionalLight directionalLight;
uniform vec4 tintColor;
uniform vec4 ambientColor;
uniform vec3 cameraPosition;
uniform vec3 cameraDirection;
uniform vec2 screenResolution;

vec4 APPLY_DIRECTIONAL_LIGHT(vec4 color)
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight.direction);

    // Diffuse (Lambert)
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(cameraPosition - Position); // assuming you pass these
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), 16);

    // Ambient term (to avoid pitch-black areas)
    vec3 ambient = 0.1 * color.rgb;

    // Combine
    vec3 diffuse = diff * color.rgb * directionalLight.color.rgb;
    vec3 specular = spec * directionalLight.color.rgb * directionalLight.intensity;

    return vec4(ambient + diffuse + specular, color.a);
}

vec4 APPLY_AMBIENT(vec4 color, vec4 textureColor)
{
    vec3 ambient = ambientColor.rgb * textureColor.rgb; 
    return vec4(color.rgb + ambient, textureColor.a * color.a);
}

vec4 APPLY_TINT(vec4 color)
{
	color = color * tintColor;
	return color;
}

vec4 APPLY_FOG(vec4 color)
{
	vec4 fogColor = vec4(0.5, 0.5, 0.5, 1);

	float dist = length(cameraPosition - Position);

	float minDistance = 1;
	float maxDistance = 25;

	float t = clamp(max(dist - minDistance, 0) / (maxDistance - minDistance), 0, 1);

	color = mix(color, fogColor, t * t);
	return color;
}

vec3 CameraPosition()
{
	vec3 camPos = cameraPosition;
	return camPos;
}

vec3 CameraDirection()
{
	vec3 dir = cameraDirection;
	dir = normalize(dir);
	return dir;
}

float MainLight()
{
	vec3 normal = normalize(Normal);

	vec3 lightDir = normalize(-directionalLight.direction);

	float light = sqrt(max(dot(normal, lightDir), 0.0));

	return light;
}


float saturate(float value)
{
	return clamp(value, 0.0, 1.0);
}
	
float map(float value, float min1, float max1, float min2, float max2) 
{
	value = clamp(value, min1, max1);
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float map01(float value, float min, float max) 
{
	return map(value, min, max, 0.0, 1.0);
}

float lerp(float a, float b, float value)
{
	return mix(a, b, value);
}

vec3 lerp(vec3 a, vec3 b, float value)
{
	return mix(a, b, value);
}

vec4 lerp(vec4 a, vec4 b, float value)
{
	return mix(a, b, value);
}