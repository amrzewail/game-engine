#pragma once
#include <cassert>

#include <iostream>

struct BoneWeight
{
public:
	static const int NUM_BONES_PER_VERTEX = 4;

	int ids[NUM_BONES_PER_VERTEX];
	float weights[NUM_BONES_PER_VERTEX];

	BoneWeight()
	{
		for (int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			weights[i] = 0.0;
			ids[i] = -1;
		}
	}


	BoneWeight& operator =(const BoneWeight& rhs)
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

	BoneWeight(const BoneWeight& source)
	{
		for (int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			ids[i] = source.ids[i];
			weights[i] = source.weights[i];
		}
	}


	void AddBoneData(int boneId, float weight)
	{
		if (weight < 0.0001) return;

		int minWeightIndex = 0;

		for (int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			if (weights[i] < weights[minWeightIndex]) minWeightIndex = i;

			if (ids[i] < 0)
			{
				ids[i] = boneId;
				weights[i] = weight;
				return;
			}
		}

		if (weight > weights[minWeightIndex])
		{
			ids[minWeightIndex] = boneId;
			weights[minWeightIndex] = weight;
		}
	}

	float Sum()
	{
		float sum = 0;
		for (int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			sum += weights[i];
		}
		return sum;
	}
};

