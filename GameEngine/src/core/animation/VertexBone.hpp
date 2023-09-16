#pragma once
struct VertexBone
{
public:
	static const int NUM_BONES_PER_VERTEX = 4;

	int ids[NUM_BONES_PER_VERTEX];
	float weights[NUM_BONES_PER_VERTEX];

	VertexBone()
	{
	}


	VertexBone& operator =(const VertexBone& rhs)
	{
		if (this != &rhs)
		{
			for (int i = 0; i < NUM_BONES_PER_VERTEX; i++)
			{
				ids[i] = rhs.ids[i];
				weights[i] = rhs.weights[i];
			}
		}
		return *this;
	}

	VertexBone(const VertexBone& source)
	{
		for (int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			ids[i] = source.ids[i];
			weights[i] = source.weights[i];
		}
	}
};

