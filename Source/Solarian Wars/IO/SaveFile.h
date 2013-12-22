////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Urho3d\Resource.h>

#define THUMBNAIL_SIZE (64 * 64) * 3
#define MAX_LENGTH 256

struct SaveHeader
{
    char characterName[MAX_LENGTH];
    char corporationName[MAX_LENGTH];
    char currentSystem[MAX_LENGTH];
    char thumbnail[THUMBNAIL_SIZE];
};

struct SaveData
{
    char* data;
};

class SaveFile :  public Urho3D::Resource
{
    OBJECT(SaveFile)
    BASEOBJECT(Resource)

public:
    SaveFile(Urho3D::Context* context);

    ///Read the Save Header
    bool LoadHeader(Urho3D::Deserializer& source);
    ///Read the Save Data
    virtual bool Load(Urho3D::Deserializer& source);
    ///Create a new Save including Header and Data
    virtual bool Save(Urho3D::Serializer& dest) const;

    ///Get The Header
    const SaveHeader& GetHeader();
    ///Get The Data
    const SaveData& GetData();

    ///Set The Header
    void SetHeader(const SaveHeader& header);
    ///Set The Data
    void SetData(const SaveData& data);

private:
    SaveHeader m_Header;
    SaveData m_Data;
};

