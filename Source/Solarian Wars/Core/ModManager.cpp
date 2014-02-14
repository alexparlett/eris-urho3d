////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Events.h"
#include "ModManager.h"
#include "IO/Settings.h"

#include <FileSystem.h>
#include <File.h>
#include <XMLFile.h>
#include <ForEach.h>
#include <Log.h>
#include <ResourceCache.h>

using namespace Urho3D;

ModManager::ModManager(Context* context) : 
    Object(context)
{
    SubscribeToEvent(E_MOD_ACTIVATED, HANDLER(ModManager, ModActivated));
    SubscribeToEvent(E_MOD_DEACTIVATED, HANDLER(ModManager, ModDeactivated));
    SubscribeToEvent(E_MOD_ORDER_SAVED, HANDLER(ModManager, ModOrderSaved));
}

void ModManager::Load()
{
    FileSystem* fs = GetSubsystem<FileSystem>();
    Settings* sets = GetSubsystem<Settings>();

    ScanDirectory(String("Mods/"));
    ScanDirectory(sets->GetSetting("userdir").GetString() + "Mods/");

    String orderFilename = sets->GetSetting("userdir").GetString() + "modorder.xml";
    if (fs->FileExists(orderFilename))
    {
        File orderFile(context_, orderFilename);
        if (orderFile.IsOpen())
        {
            XMLFile orderXml(context_);
            orderXml.Load(orderFile);

            XMLElement root = orderXml.GetRoot("mods");
            XPathResultSet mods = root.Select("mod");
            for (unsigned int i = 0; i < mods.Size(); i++)
            {
                //Required due to conventions of for loops in XPathResultSet.
                XMLElement mod = mods[i];
                String value = mod.GetValue();

                if (!value.Empty() && modDescriptors_.Contains(value))
                {
                    Activate(value);
                }
            }
        }
    }

    ActivateMods();
}

void ModManager::Save()
{
    String orderFilename = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/modorder.xml";
    File orderFile(context_, orderFilename, FILE_WRITE);
    if (orderFile.IsOpen())
    {
        XMLFile orderXml(context_);
        XMLElement root = orderXml.CreateRoot("mods");
        foreach(String id, activeMods_)
       { 
            root.CreateChild("mod").SetValue(id);
        }

        if (!orderXml.Save(orderFile))
        {
            LOGERROR("Unable to save mod order " + orderFilename);
        }
    }
    else
    {
        LOGERROR("Unable to open mod order " + orderFilename);
    }
}

const Urho3D::HashMap<Urho3D::String, Mod>& ModManager::GetModDescriptors() const
{
    return modDescriptors_;
}

bool ModManager::IsActive(Urho3D::String id) const
{
    return activeMods_.Contains(id);
}

void ModManager::ModActivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace ModActivated;
    String id = eventData[P_ID].ToString();
    unsigned int priority = eventData[P_PRIORITY].GetUInt();

    Activate(id, priority);
}

void ModManager::ModDeactivated(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace ModDeactivated;
    String id = eventData[P_ID].ToString();

    if (activeMods_.Contains(id))
    {
        // Remove it from mod order.
        activeMods_.Remove(id);

        // Remove it from the ResourceCache, don't need to release resources here as that will be done when the mod order is saved.
        GetSubsystem<ResourceCache>()->RemoveResourceDir(modDescriptors_[id].GetDirectory() + "/Data");
    }
}

void ModManager::ModOrderSaved(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    ActivateMods();
    Save();
}

void ModManager::Activate(const Urho3D::String& id, unsigned int priorty)
{
    if (!activeMods_.Contains(id) && modDescriptors_.Contains(id))
    {
        // If low priority add to the end.
        if (priorty == PRIORITY_LOW)
            priorty = activeMods_.Size();

        // Add it to mod order.
        activeMods_.Insert(priorty, id);
    }
}

void ModManager::ScanDirectory(Urho3D::String& root)
{
    unsigned int slashPos = root.FindLast('/');
    if (slashPos == String::NPOS || slashPos < root.Length() - 1)
    {
        root.Append('/');
    }

    Vector<String> dirList;

    FileSystem* fs = GetSubsystem<FileSystem>();
    fs->ScanDir(dirList, root, "", SCAN_DIRS, false);

    // Handle removing the relative dirs.
    dirList.Remove(".");
    dirList.Remove("..");

    foreach(String dir, dirList)
    {
        if (fs->FileExists(root + dir + "/mod.xml"))
        {
            File file(context_, root + dir + "/mod.xml");

            if (file.IsOpen())
            {
                XMLFile descriptor(context_);
                descriptor.Load(file);

                Mod mod(dir, descriptor);

                modDescriptors_[mod.GetId()] = mod;
            }
        }
    }
}

void ModManager::ActivateMods()
{
    ResourceCache* rc = GetSubsystem<ResourceCache>();
    rc->ReleaseAllResources();

    for (unsigned i = activeMods_.Size() - 1; activeMods_.Size() > 0 && i >= 0; i--)
    {
        HandlePatches(modDescriptors_[activeMods_[i]].GetDirectory() + "/Patch/");
        rc->AddResourceDir(modDescriptors_[activeMods_[i]].GetDirectory() + "/Data");
    }
}

void ModManager::HandlePatches(String& root)
{
    Vector<String> fileList;
    FileSystem* fs = GetSubsystem<FileSystem>();

    fs->ScanDir(fileList, root, "*.xml", SCAN_FILES, true);

    foreach(String file, fileList)
    {
        File patchFile(context_, root + file);
        if (patchFile.IsOpen())
        {
            XMLFile patch(context_);
            patch.Load(patchFile);

            XMLFile* res = GetSubsystem<ResourceCache>()->GetResource<XMLFile>(file);
            if (res)
                res->Patch(&patch);
        }
    }
}
