#include "Galaxy.h"

#include <Context.h>
#include <ScriptInstance.h>
#include <ResourceCache.h>

using namespace Urho3D;

Galaxy::Galaxy(Context* context) :
	Component(context),
	systems_(HashMap<String, SharedPtr<System> >())
{
}

Galaxy::~Galaxy()
{
}

System* Galaxy::CreateSystem(const SystemProperties& properties)
{
	Scene* scene = GetScene();

	Node* systemNode = scene->CreateChild(properties.GetName());
	systemNode->SetPosition(properties.GetPosition());

	System* system = systemNode->CreateComponent<System>();
	systems_[properties.GetName()] = SharedPtr<System>(system);

	ScriptInstance* script = systemNode->CreateComponent<ScriptInstance>();
	script->CreateObject(GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Controllers/SystemController.as"), "SystemController");

	return system;
}

System* Galaxy::GetSystem(const String& name) const
{
	HashMap<String, SharedPtr<System> >::ConstIterator find = systems_.Find(name);
	if (find != systems_.End())
		return find->second_;
	return NULL;
}