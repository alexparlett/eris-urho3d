////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <XMLFile.h>
#include <Scene.h>
#include <Str.h>
#include <Image.h>

#define THUMBNAIL_SIZE (128 * 3) * 128
#define MAX_LENGTH 64

struct SaveHeader
{
    char name[MAX_LENGTH];
    char date[MAX_LENGTH];
    char time[MAX_LENGTH];
    char characterName[MAX_LENGTH];
    char corporationName[MAX_LENGTH];
    char currentSystem[MAX_LENGTH];
    char thumbnail[THUMBNAIL_SIZE];
};

class SaveFile :  public Urho3D::Object
{
    OBJECT(SaveFile)

    friend class SaveManager;

public:
    SaveFile(Urho3D::Context* context);

    ///Read the Save Data
    bool Load(const Urho3D::String& source);
    ///Create a new Save including Header and Data
    bool Save(const Urho3D::String& dest, Urho3D::Scene* scene, const Urho3D::String& character, const Urho3D::String& corporation, const Urho3D::String& system, Urho3D::Image* thumbnail);

    ///Get The Header
    const SaveHeader& GetHeader();

protected:
    ///Read the Save Header
    bool LoadHeader(const Urho3D::String& source);

private:
    SaveHeader m_Header;
};

