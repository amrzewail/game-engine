#pragma once

#include "Bone.h"
#include <map>
#include <iostream>
#include <sstream>
#include <stack>

class Armature
{
private:
	std::map<std::string, int> _boneMapping;
	std::vector<Bone*> _bones;
	Matrix4x4 globalInverseTransform;

public:

	~Armature()
	{
		for (auto b : _bones)
		{
			delete b;
		}

		_bones.clear();
	}

	Armature()
	{

	}

	Armature(Matrix4x4 globalInverseTransform)
	{
		this->globalInverseTransform = globalInverseTransform;
	}

	const Matrix4x4 GlobaInverseTransform()
	{
		return this->globalInverseTransform;
	}

	int AddBone(const std::string name, const Bone bone)
	{
		Bone* b = new Bone();
		*b = bone;
		_bones.push_back(b);
		b->index = _bones.size() - 1;
		_boneMapping.emplace(name, b->index);
		return b->index;
	}

	Bone* GetBoneByName(const std::string name)
	{
		if (_boneMapping.find(name) != _boneMapping.end())
		{
			return GetBone(_boneMapping[name]);
		}
		return nullptr;
	}

	Bone* GetBone(const int index)
	{
		return _bones[index];
	}

	void FindBoneChildren(const int index, std::vector<int>& result)
	{
		for (int i = 0; i < _bones.size(); i++)
		{
			if (_bones[i]->parentIndex != index) continue;
			result.push_back(i);
		}
	}

	std::vector<Bone*> GetBones()
	{
		return _bones;
	}


	std::string ToString()
	{
		std::ostringstream oss;

		if (_bones.empty())
			return "<Empty Armature>";

		std::vector<int> roots;
		for (int i = 0; i < _bones.size(); ++i)
		{
			if (_bones[i]->parentIndex < 0)
				roots.push_back(i);
		}

		std::stack<std::pair<int, int>> stack;
		for (auto r : roots)
			stack.push({ r, 0 });

		while (!stack.empty())
		{
			auto [idx, depth] = stack.top();
			stack.pop();

			// Indentation
			for (int i = 0; i < depth; ++i) oss << "  ";
			oss << _bones[idx]->name << "\n";

			// Push children (reverse to preserve left→right order)
			std::vector<int> children;
			FindBoneChildren(idx, children);
			for (int i = (int)children.size() - 1; i >= 0; --i)
			{
				stack.push({ children[i], depth + 1 });
			}
		}

		return oss.str();
	}
};