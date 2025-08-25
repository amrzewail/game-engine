#version 330 core

struct TransformMatrices
{
	mat4 model;
	mat4 pv;
	mat4 rotation;
};

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in ivec4 inBoneIds;
layout (location = 4) in vec4 inBoneWeights;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

out vec4 BoneWeights;

uniform TransformMatrices transformMatrices;
uniform mat4 boneMatrices[128];


vec4 APPLY_TRANSFORMATION()
{
    mat4 skinMatrix = mat4(0.0);
    bool hasWeights = false;

//    for (int i = 0; i < 4; i++) 
//    {
//        int boneIndex = inBoneIds[i];
//        float weight  = inBoneWeights[i];
//        if (boneIndex >= 0 && weight > 0.0) 
//        {
//            skinMatrix += boneMatrices[boneIndex] * weight;
//            hasWeights = true;
//        }
//    }

    if (!hasWeights) 
    {
        skinMatrix = mat4(1.0);
    }

    vec4 skinnedPosition = skinMatrix * vec4(inPosition, 1.0);
    vec3 skinnedNormal   = mat3(skinMatrix) * inNormal;

	Position = vec3(transformMatrices.model * skinnedPosition);
	Normal = normalize(mat3(transformMatrices.model) * skinnedNormal);
	TexCoord = inTexCoord;

	return transformMatrices.pv * transformMatrices.model * skinnedPosition;
}
