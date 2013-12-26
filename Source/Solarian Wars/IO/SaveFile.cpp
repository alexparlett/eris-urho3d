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
thumbnail_(NULL),
length_(0),
context_(NULL)
{
}

SaveHeader::SaveHeader(Context* context) :
thumbnail_(NULL),
length_(0),
context_(context)
{
}

bool SaveHeader::Load(const String& source)
{
    FileSystem* fs = context_->GetSubsystem<FileSystem>();
    File file(context_, source);

    if (file.IsOpen())
    {
        SetName(file.GetName().Split('.')[0]);
        SetDateCreated(fs->GetLastModifiedTime(source));
        SetPlayedTime(file.ReadUInt());
        SetCharacterName(file.ReadString());
        SetCorporationName(file.ReadString());
        SetCurrentSystem(file.ReadString());

        unsigned char* buffer = NULL;
        length_ += file.Read(buffer, (128 * 3) * 128);
        Image* thumb(new Image(context_));
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
    return name_;
}

const Urho3D::String& SaveHeader::GetDateCreated() const
{
    return dateCreated_;
}

const Urho3D::String& SaveHeader::GetPlayedTime() const
{
    return playedTime_;
}

const Urho3D::String& SaveHeader::GetCharacterName() const
{
    return characterName_;
}

const Urho3D::String& SaveHeader::GetCorporationName() const
{
    return corporationName_;
}

const Urho3D::String& SaveHeader::GetCurrentSystem() const
{
    return currentSystem_;
}

const Urho3D::Image* SaveHeader::GetThumbnail() const
{
    return thumbnail_;
}

unsigned int SaveHeader::GetLength() const
{
    return length_;
}

void SaveHeader::SetName(const Urho3D::String& name)
{
    name_ = name;
}

void SaveHeader::SetDateCreated(unsigned int dateCreated)
{
    char* buffer = NULL;
    time_t t = static_cast<time_t>(dateCreated);
    struct tm timeinfo;
    localtime_s(&timeinfo, &t);
    strftime(buffer, 128, "%Y-%m-%d %H:%M:%S", &timeinfo);
    dateCreated_ = String(buffer);
    delete buffer;
}

void SaveHeader::SetPlayedTime(unsigned int playedTime)
{
    char* buffer = NULL;
    time_t t = static_cast<time_t>(playedTime);
    struct tm timeinfo;
    localtime_s(&timeinfo, &t);
    strftime(buffer, 128, "%H:%M:%S", &timeinfo);
    playedTime_ = String(buffer);
    delete buffer;

    length_ += sizeof(playedTime);
}

void SaveHeader::SetCharacterName(const Urho3D::String& characterName)
{
    characterName_ = characterName;
    length_ += characterName_.Length() + 1;
}

void SaveHeader::SetCorporationName(const Urho3D::String& corporationName)
{
    corporationName_ = corporationName;
    length_ += corporationName_.Length() + 1;
}

void SaveHeader::SetCurrentSystem(const Urho3D::String& currentSystem)
{
    currentSystem_ = currentSystem;
    length_ += currentSystem_.Length() + 1;
}

void SaveHeader::SetThumbnail(Urho3D::Image* thumbnail)
{
    thumbnail_ = SharedPtr<Image>(thumbnail);
    length_ += sizeof(thumbnail_->GetData());
}

void SaveHeader::SetLength(unsigned int length)
{
    length_ = length;
}

void SaveHeader::SetContext(Urho3D::Context* context)
{
    context_ = context;
}
