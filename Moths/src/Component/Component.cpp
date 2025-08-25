#include <Moths/Component/Component.h>
#include <Moths/Core/Guid.h>

using namespace MothsEngine;

Component::Component()
{
	runtimeGuid = Guid::NewGuid();
}

Component::~Component()
{
	delete runtimeGuid;
}

void Component::Awake()
{}

void Component::Start()
{}

void Component::Update(float deltaTime)
{}

void Component::Render(Camera& targetCamera) const
{}

void Component::OnDestroy()
{}
