////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <Input.h>
#include <HashMap.h>
#include <Str.h>
#include <XMLElement.h>
#include <Ptr.h>

class Bindings : public Urho3D::Object
{
    OBJECT(Bindings)

public:
    Bindings(Urho3D::Context* context);

    void Load();
    void Save();

    int GetActionScanCode(const Urho3D::String& action) const;
    void SetActionScanCode(const Urho3D::String& action, int scanCode);
    void SetActionScanCodeFromKey(const Urho3D::String& action, int keyCode);

private:
    Urho3D::HashMap<Urho3D::String, int> actionMap_;
    Urho3D::WeakPtr<Urho3D::Input> input_;
};

