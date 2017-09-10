////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Core/Object.h>
#include <Container/Ptr.h>
#include <Container/HashMap.h>
#include <Resource/XMLElement.h>

class Page : public Urho3D::RefCounted
{
public:
    Page() : RefCounted() {}
    Page(Urho3D::XMLElement& page);

    Urho3D::String GetLine(unsigned line) const;

private:
    Urho3D::HashMap<unsigned, Urho3D::String> lines_;
};

class Locale : public Urho3D::Object
{
    URHO3D_OBJECT(Locale, Urho3D::Object)

public:
    Locale(Urho3D::Context* context);
    virtual ~Locale();

    void Load(const Urho3D::String& fileName);

    Urho3D::String Localize(unsigned page, unsigned line) const;
    void Replace(Urho3D::String& line, unsigned token, const Urho3D::String& value) const;
    void Replace(Urho3D::String& line, const Urho3D::Vector<Urho3D::String>& values) const;

private:
    Urho3D::HashMap<unsigned, Urho3D::SharedPtr<Page>> pages_;
};

