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

class SaveHeader
{
private:
    Urho3D::String m_Name;
    Urho3D::String m_DateCreated;
    Urho3D::String m_PlayedTime;
    Urho3D::String m_CharacterName;
    Urho3D::String m_CorporationName;
    Urho3D::String m_CurrentSystem;
    Urho3D::Image* m_Thumbnail;

    unsigned int m_Length;
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

