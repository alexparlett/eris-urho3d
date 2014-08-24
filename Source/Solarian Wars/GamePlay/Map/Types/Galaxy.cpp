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
	Scene* scene = GetScene();

	Node* systemNode = scene->CreateChild(properties.name_);
	systemNode->SetPosition(properties.position_);

	System* system = systemNode->CreateComponent<System>();
	systems_[properties.name_] = SharedPtr<System>(system);

	ScriptInstance* script = systemNode->CreateComponent<ScriptInstance>();
    ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>("Scripts/Controllers/SystemController.as");
    if (file)
        script->CreateObject(file, "SystemController");
    else
        LOGERRORF("SystemController ScriptFile[%s] does not exists.", "Scripts/Controllers/SystemController.as");    

    return system;
}

System* Galaxy::GetSystem(const String& name) const
{
	HashMap<String, SharedPtr<System> >::ConstIterator find = systems_.Find(name);
	if (find != systems_.End())
		return find->second_;
	return NULL;
}