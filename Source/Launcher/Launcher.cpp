////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Launcher.h"

#include <FileSystem.h>
#include <Log.h>

using namespace Urho3D;

Launcher::Launcher(Context* context) :
Application(context)
{
}

void Launcher::Setup()
{
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetUserDocumentsDir() + "My Games/Solarian Wars/launcher.log";
    engineParameters_["Headless"] = true;
}

void Launcher::Start()
{
    if (Is64BitOS())
    {
        LOGINFO("Starting SolarianWars_64.exe");
        GetSubsystem<FileSystem>()->SystemCommand("start SolarianWars_64.exe");
        GetSubsystem<Engine>()->Exit();
    }
    else
    {
        LOGINFO("Starting SolarianWars.exe");
        GetSubsystem<FileSystem>()->SystemCommand("start SolarianWars.exe");
        GetSubsystem<Engine>()->Exit();
    }
}

BOOL Launcher::Is64BitOS()
{
    BOOL bIs64BitOS = FALSE;

    // We check if the OS is 64 Bit
    typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");

    if (fnIsWow64Process != NULL)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIs64BitOS))
        {
            LOGERROR("Unable to determine if 64bit, starting 32 bit SolarianWars.exe");
        }
    }
    return bIs64BitOS;
}

DEFINE_APPLICATION_MAIN(Launcher)