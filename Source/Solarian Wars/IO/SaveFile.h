////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Context.h>
#include <Str.h>
#include <Image.h>
#include <File.h>
#include <Ptr.h>

class SaveHeader
{
public:
    SaveHeader();
    SaveHeader(Urho3D::Context* context);

    bool Load(const Urho3D::String& source);
    bool Save(Urho3D::File& file);

    const Urho3D::String& GetName() const;
    const Urho3D::String& GetDateCreated() const;
    const Urho3D::String& GetPlayedTime() const;
    const Urho3D::String& GetCharacterName() const;
    const Urho3D::String& GetCorporationName() const;
    const Urho3D::String& GetCurrentSystem() const;
    const Urho3D::Image* GetThumbnail() const;
    unsigned int GetLength() const;

    void SetName(const Urho3D::String& name);
    void SetDateCreated(unsigned int dateCreated);
    void SetPlayedTime(unsigned int playedTime);
    void SetCharacterName(const Urho3D::String& characterName);
    void SetCorporationName(const Urho3D::String& corporationName);
    void SetCurrentSystem(const Urho3D::String& currentSystem);
    void SetThumbnail(Urho3D::Image* thumbnail);
    void SetLength(unsigned int length);
    void SetContext(Urho3D::Context* context);

private:
    Urho3D::Context* m_Context;

    Urho3D::String m_Name;
    Urho3D::String m_DateCreated;
    Urho3D::String m_PlayedTime;
    Urho3D::String m_CharacterName;
    Urho3D::String m_CorporationName;
    Urho3D::String m_CurrentSystem;
    Urho3D::SharedPtr<Urho3D::Image> m_Thumbnail;

    unsigned int m_Length;
};