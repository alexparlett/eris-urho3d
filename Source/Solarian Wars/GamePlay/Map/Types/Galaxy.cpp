#include "Galaxy.h"

#include <Context.h>
#include <ScriptFile.h>
#include <ScriptInstance.h>
#include <ResourceCache.h>
#include <Log.h>

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
	Node* systemNode = GetNode()->CreateChild(properties.name_);
	systemNode->SetPosition(properties.position_);

	ScriptInstance* script = systemNode->CreateComponent<ScriptInstance>();
    ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>(properties.controller_);
	if (file)
		script->CreateObject(file, "SystemController");
	else
	{
		systemNode->Remove();
		return NULL;
	}

	System* system = systemNode->CreateComponent<System>();
	systems_[properties.name_] = SharedPtr<System>(system);

    return system;
}

System* Galaxy::GetSystem(const String& name) const
{
	HashMap<String, SharedPtr<System> >::ConstIterator find = systems_.Find(name);
	return find != systems_.End() ? find->second_ : NULL;
}