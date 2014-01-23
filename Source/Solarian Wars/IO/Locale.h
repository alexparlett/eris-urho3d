////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <Ptr.h>
#include <HashMap.h>
#include <XMLElement.h>

class Page : public Urho3D::RefCounted
{
public:
    Page() {}
    Page(Urho3D::XMLElement& page);

    Urho3D::String GetLine(int line) const;

private:
    Urho3D::HashMap<int, Urho3D::String> lines_;
};

class Locale : public Urho3D::Object
{
    OBJECT(Locale)

public:
    Locale(Urho3D::Context* context);

    void Load(const Urho3D::String& fileName);

    Urho3D::String Localize(int page, int line) const;
    void Replace(Urho3D::String& line, const Urho3D::String& value) const;
    void Replace(Urho3D::String& line, const Urho3D::PODVector<Urho3D::String>& values) const;

private:
    Urho3D::HashMap<int, Urho3D::SharedPtr<Page>> pages_;
};

