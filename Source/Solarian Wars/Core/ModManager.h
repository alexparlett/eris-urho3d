////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Math/MathDefs.h>
#include <Core/Object.h>
#include <Container/HashMap.h>
#include <Math/StringHash.h>
#include <Container/Vector.h>

#include "IO\Module.h"

#define PRIORITY_LOW Urho3D::M_MAX_UNSIGNED
#define PRIORITY_HIGH Urho3D::M_MIN_UNSIGNED

class ModManager : public Urho3D::Object
{
	URHO3D_OBJECT(ModManager, Urho3D::Object)

public:
    ModManager(Urho3D::Context* context);

    void Load();
    void Save();

    const Urho3D::HashMap<Urho3D::String, Module>& GetModDescriptors() const;
    bool IsActive(Urho3D::String id) const;

private:
    void HandleActivate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleDeactivate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleSave(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    void Activate(const Urho3D::String& id, unsigned int priorty = PRIORITY_LOW);

    void ScanDirectory(Urho3D::String& root);
    void ActivateMods();
    void HandlePatches(Urho3D::String& root);

    Urho3D::PODVector<Urho3D::String> activeMods_;
    Urho3D::HashMap<Urho3D::String,Module> modDescriptors_;
};

