#include <Moths/Entity/GameObject.h>

namespace MothsEngine
{
	GameObject::GameObject()
	{
		_components = std::unordered_map<std::string, std::vector<Component*>>();
		transform = new Transform();
	}


	GameObject::~GameObject()
	{
		for (auto& pair : _components)
		{
			for (auto c : pair.second)
			{
				c->OnDestroy();
				delete c;
			}
		}

		delete transform;
	}
}