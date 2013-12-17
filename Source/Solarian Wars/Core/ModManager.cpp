////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "ModManager.h"

#include <FileSystem.h>
#include <File.h>
#include <XMLFile.h>

using namespace Urho3D;

ModManager::ModManager(Context* context) : 
Object(context),
m_ActiveMods(Vector<StringHash>()),
m_ModDescriptors(HashMap<StringHash, Mod>())
{
}

void ModManager::Initialize()
{
    Vector<String> dirList;

    FileSystem* fs = GetSubsystem<FileSystem>();
    fs->ScanDir(dirList, "Mods", "", SCAN_DIRS, false);

    // Handle removing the relative dirs.
    dirList.Remove(".");
    dirList.Remove("..");

    for (int i = 0; i < dirList.Size(); i++)
    {
        Vector<String> descriptorFile;
        fs->ScanDir(descriptorFile, "Mods/" + dirList[i], "mod.xml", SCAN_FILES, false);

        if (descriptorFile.Size() > 0)
        {
            File file(context_, "Mods/" + dirList[i] + "/" + descriptorFile[0]);
            
            if (file.IsOpen())
            {
                XMLFile descriptor(context_);
                descriptor.Load(file);

                Mod mod(dirList[i], descriptor);

                m_ModDescriptors[mod.GetId()] = mod;
            }
        }
    }
}

void ModManager::ModActivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{

}

void ModManager::ModDeactivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{

}

void ModManager::ModOrderChanged(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{

}