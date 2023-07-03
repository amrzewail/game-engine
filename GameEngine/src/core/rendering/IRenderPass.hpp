#pragma once


class IRenderPass
{

public:

	unsigned int activeTextureIndex;
	unsigned int frameBuffer;
	unsigned int textureBuffer;


	virtual ~IRenderPass() {}

	virtual void Initialize(int index) = 0;

	virtual void Render(Camera& camera) = 0;

};