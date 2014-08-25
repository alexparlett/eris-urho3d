#pragma once

#include "System.h"

#include <Scene.h>
#include <Object.h>
#include <HashMap.h>
#include <Ptr.h>
#include <Variant.h>

#include "GamePlay/Map/Properties/SystemProperties.h"

class Galaxy : public Urho3D::Component
{
	OBJECT(Galaxy)

public:
    // Ctor
	Galaxy(Urho3D::Context* context);
    // Dtor
	virtual ~Galaxy();
    // Register Factory
    static void RegisterObject(Urho3D::Context* context);

    // Apply Attributes
    virtual void ApplyAttributes();

    // Create System Method
	System* CreateSystem(const SystemProperties& properties);
    // Get System Method
	System* GetSystem(const Urho3D::String& name) const;

    // Attribute Saving
    void SetSystemIdsAttr(const Urho3D::VariantVector& value);
    // Attribute Saving
    const Urho3D::VariantVector& GetSystemIdsAttr() const { return systemIdsAttr_; }

private:
	Urho3D::HashMap<Urho3D::String, Urho3D::SharedPtr<System> > systems_;
    mutable Urho3D::VariantVector systemIdsAttr_;
};

