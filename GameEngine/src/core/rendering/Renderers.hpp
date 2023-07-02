#pragma once

#include <vector>
#include "Renderer.hpp"
#include <algorithm>

class Renderers
{
private:
	static std::vector<Renderer*>* _renderers;

public:
	static void AddRenderer(Renderer* renderer)
	{
		if (!_renderers)
		{
			_renderers = new std::vector<Renderer*>();
		}
		_renderers->push_back(renderer);
	}

	static void RemoveRenderer(Renderer* renderer)
	{
		if (!_renderers)
		{
			return;
		}

		for (int i = 0; i < _renderers->size(); i++)
		{
			if ((*_renderers)[i] == renderer)
			{
				_renderers->erase(_renderers->begin() + i);
				break;
			}
		}
	}

	static bool SortCompare(Renderer* left, Renderer* right)
	{
		return *(left->priority) < *(right->priority);
	}

	static std::vector<Renderer*>& GetRenderers()
	{
		if (!_renderers)
		{
			_renderers = new std::vector<Renderer*>();
		}

		std::sort(_renderers->begin(), _renderers->end(), SortCompare);
		return *_renderers;
	}
};