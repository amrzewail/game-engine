#pragma once

struct DrawDetails
{
	DrawDetails(uint32_t v, uint32_t e)
	{
		vao = v;
		numElements = e;
	}

	uint32_t vao = 0;
	uint32_t numElements = 0;
};