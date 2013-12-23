////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <XMLFile.h>
#include <Scene.h>

#define THUMBNAIL_SIZE (64 * 64) * 3
#define MAX_LENGTH 256

struct SaveHeader
{
    char characterName[MAX_LENGTH];
    char corporationName[MAX_LENGTH];
    char currentSystem[MAX_LENGTH];
    char thumbnail[THUMBNAIL_SIZE];
};

class SaveFile :  public Urho3D::Object
{
    OBJECT(SaveFile)

public:
    SaveFile(Urho3D::Context* context);

    ///Read the Save Header
    bool LoadHeader(const Urho3D::String& source);
    ///Read the Save Data
    virtual bool Load(const Urho3D::String& source);
    ///Create a new Save including Header and Data
    virtual bool Save(const Urho3D::String& dest, Urho3D::Scene* scene) const;

    ///Get The Header
    const SaveHeader& GetHeader();

    ///Set The Header
    void SetHeader(const SaveHeader& header);

private:
    SaveHeader m_Header;
};

