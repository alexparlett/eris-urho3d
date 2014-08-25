#include "Galaxy.h"

#include <Context.h>
#include <Scene.h>
#include <ScriptFile.h>
#include <ScriptInstance.h>
#include <ResourceCache.h>
#include <Log.h>

using namespace Urho3D;

Galaxy::Galaxy(Context* context) :
	Component(context),
	systems_(HashMap<String, SharedPtr<System> >()),
    systemIdsAttr_(VariantVector())
{
}

Galaxy::~Galaxy()
{
}

void Galaxy::RegisterObject(Context* context)
{
    context->RegisterFactory<Galaxy>("GAME_CATEGORY");

    REF_ACCESSOR_ATTRIBUTE(Galaxy, VAR_VARIANTVECTOR, "Systems", GetSystemIdsAttr, SetSystemIdsAttr, VariantVector, Variant::emptyVariantVector, AM_FILE | AM_NODEIDVECTOR);
}


void Galaxy::ApplyAttributes()
{
    systems_.Clear();

    Scene* scene = GetScene();

    if (scene)
    {
        // The first index stores the number of IDs redundantly. This is for editing
        for (unsigned i = 1; i < systemIdsAttr_.Size(); ++i)
        {
            Node* node = scene->GetNode(systemIdsAttr_[i].GetUInt());
            if (node)
            {
                systems_[node->GetName()] = node->GetComponent<System>();
            }
        }
    }
}


System* Galaxy::CreateSystem(const SystemProperties& properties)
{
	Node* systemNode = GetNode()->CreateChild(properties.GetName());
	systemNode->SetPosition(properties.GetPosition());

	ScriptInstance* script = systemNode->CreateComponent<ScriptInstance>();
    ScriptFile* file = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>(properties.GetController());
    if (!file || (file && !script->CreateObject(file, "SystemController")))
    { 
		systemNode->Remove();
		return NULL;
	}

	System* system = systemNode->CreateComponent<System>();
	systems_[properties.GetName()] = SharedPtr<System>(system);
    systemIdsAttr_.Push(systemNode->GetID());

    return system;
}

System* Galaxy::GetSystem(const String& name) const
{
	HashMap<String, SharedPtr<System> >::ConstIterator find = systems_.Find(name);
	return find != systems_.End() ? find->second_.Get() : NULL;
}

void Galaxy::SetSystemIdsAttr(const VariantVector& value)
{
    // Just remember the node IDs. They need to go through the SceneResolver, and we actually find the nodes during
    // ApplyAttributes()
    if (value.Size())
    {
        systemIdsAttr_.Clear();

        unsigned index = 0;
        unsigned numInstances = value[index++].GetUInt();
        // Prevent crash on entering negative value in the editor
        if (numInstances > M_MAX_INT)
            numInstances = 0;

        systemIdsAttr_.Push(numInstances);
        while (numInstances--)
        {
            // If vector contains less IDs than should, fill the rest with zeros
            if (index < value.Size())
                systemIdsAttr_.Push(value[index++].GetUInt());
            else
                systemIdsAttr_.Push(0);
        }
    }
    else
    {
        systemIdsAttr_.Clear();
        systemIdsAttr_.Push(0);
    }
}
