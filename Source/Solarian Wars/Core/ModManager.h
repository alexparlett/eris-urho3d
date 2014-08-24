////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <MathDefs.h>
#include <Object.h>
#include <HashMap.h>
#include <StringHash.h>
#include <Vector.h>

#include "IO\Mod.h"

#define PRIORITY_LOW Urho3D::M_MAX_UNSIGNED
#define PRIORITY_HIGH Urho3D::M_MIN_UNSIGNED

class ModManager : public Urho3D::Object
{
    OBJECT(ModManager)

public:
    ModManager(Urho3D::Context* context);

    void Load();
    void Save();

    const Urho3D::HashMap<Urho3D::String, Mod>& GetModDescriptors() const;
    bool IsActive(Urho3D::String id) const;

private:
    void ModActivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ModDeactivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ModOrderSaved(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    void Activate(const Urho3D::String& id, unsigned int priorty = PRIORITY_LOW);

    void ScanDirectory(Urho3D::String& root);
    void ActivateMods();
    void HandlePatches(Urho3D::String& root);

    Urho3D::PODVector<Urho3D::String> activeMods_;
    Urho3D::HashMap<Urho3D::String,Mod> modDescriptors_;
};

