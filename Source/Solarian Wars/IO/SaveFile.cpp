////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Core\Events.h"
#include "SaveFile.h"

#include <FileSystem.h>
#include <Log.h>
#include <Timer.h>

#include <time.h>

using namespace Urho3D;

SaveHeader::SaveHeader() :
m_Name(String::EMPTY),
m_DateCreated(String::EMPTY),
m_PlayedTime(String::EMPTY),
m_CharacterName(String::EMPTY),
m_CorporationName(String::EMPTY),
m_CurrentSystem(String::EMPTY),
m_Thumbnail(NULL),
m_Length(0),
m_Context(NULL)
{
}

SaveHeader::SaveHeader(Context* context) :
m_Name(String::EMPTY),
m_DateCreated(String::EMPTY),
m_PlayedTime(String::EMPTY),
m_CharacterName(String::EMPTY),
m_CorporationName(String::EMPTY),
m_CurrentSystem(String::EMPTY),
m_Thumbnail(NULL),
m_Length(0),
m_Context(context)
{
}

bool SaveHeader::Load(const String& source)
{
    FileSystem* fs = m_Context->GetSubsystem<FileSystem>();
    File file(m_Context, source);

    if (file.IsOpen())
    {
        SetName(file.GetName().Split('.')[0]);
        SetDateCreated(fs->GetLastModifiedTime(source));
        SetPlayedTime(file.ReadUInt());
        SetCharacterName(file.ReadString());
        SetCorporationName(file.ReadString());
        SetCurrentSystem(file.ReadString());

        unsigned char* buffer = NULL;
        m_Length += file.Read(buffer, (128 * 3) * 128);
        Image* thumb(new Image(m_Context));
        thumb->SetSize(128, 128, 3);
        thumb->SetData(buffer);
        SetThumbnail(thumb);
        delete buffer;

        return true;
    }

    return false;
}

bool SaveHeader::Save(File& dest)
{
    return false;
}

const Urho3D::String& SaveHeader::GetName() const
{
    return m_Name;
}

const Urho3D::String& SaveHeader::GetDateCreated() const
{
    return m_DateCreated;
}

const Urho3D::String& SaveHeader::GetPlayedTime() const
{
    return m_PlayedTime;
}

const Urho3D::String& SaveHeader::GetCharacterName() const
{
    return m_CharacterName;
}

const Urho3D::String& SaveHeader::GetCorporationName() const
{
    return m_CorporationName;
}

const Urho3D::String& SaveHeader::GetCurrentSystem() const
{
    return m_CurrentSystem;
}

const Urho3D::Image* SaveHeader::GetThumbnail() const
{
    return m_Thumbnail;
}

unsigned int SaveHeader::GetLength() const
{
    return m_Length;
}

void SaveHeader::SetName(const Urho3D::String& name)
{
    m_Name = name;
}

void SaveHeader::SetDateCreated(unsigned int dateCreated)
{
    char* buffer = NULL;
    time_t t = static_cast<time_t>(dateCreated);
    struct tm timeinfo;
    localtime_s(&timeinfo, &t);
    strftime(buffer, 128, "%Y-%m-%d %H:%M:%S", &timeinfo);
    m_DateCreated = String(buffer);
    delete buffer;
}

void SaveHeader::SetPlayedTime(unsigned int playedTime)
{
    char* buffer = NULL;
    time_t t = static_cast<time_t>(playedTime);
    struct tm timeinfo;
    localtime_s(&timeinfo, &t);
    strftime(buffer, 128, "%H:%M:%S", &timeinfo);
    m_PlayedTime = String(buffer);
    delete buffer;

    m_Length += sizeof(playedTime);
}

void SaveHeader::SetCharacterName(const Urho3D::String& characterName)
{
    m_CharacterName = characterName;
    m_Length += m_CharacterName.Length() + 1;
}

void SaveHeader::SetCorporationName(const Urho3D::String& corporationName)
{
    m_CorporationName = corporationName;
    m_Length += m_CorporationName.Length() + 1;
}

void SaveHeader::SetCurrentSystem(const Urho3D::String& currentSystem)
{
    m_CurrentSystem = currentSystem;
    m_Length += m_CurrentSystem.Length() + 1;
}

void SaveHeader::SetThumbnail(Urho3D::Image* thumbnail)
{
    m_Thumbnail = SharedPtr<Image>(thumbnail);
    m_Length += sizeof(m_Thumbnail->GetData());
}

void SaveHeader::SetLength(unsigned int length)
{
    m_Length = length;
}

void SaveHeader::SetContext(Urho3D::Context* context)
{
    m_Context = context;
}
