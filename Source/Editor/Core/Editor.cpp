////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "Editor.h"
#include "IO/Settings.h"

#include <ResourceCache.h>
#include <FileSystem.h>
#include <CoreEvents.h>
#include <Input.h>
#include <Vector.h>
#include <XMLFile.h>
#include <ForEach.h>
#include <Renderer.h>
#include <Zone.h>
#include <Graphics.h>

using namespace Urho3D;

Editor::Editor(Context* context) :
Application(context),
m_ConfigFile(String::EMPTY),
m_Production(false),
m_Scene(SharedPtr<EditorScene>(new EditorScene(context)))
{
    SubscribeToEvent(E_UPDATE, HANDLER(Editor, HandlerUpdate));

    context->RegisterSubsystem(this);
    context->RegisterSubsystem(new Settings(context));
}

void Editor::Setup()
{
    engineParameters_["Headless"] = false;
    engineParameters_["WindowTitle"] = "Solarian Wars Editor";
    engineParameters_["WindowIcon"] = "Textures/Icon.png";
    engineParameters_["LogName"] = GetSubsystem<Settings>()->GetSetting("userdir").GetString() + "editor.log";
    engineParameters_["FullScreen"] = false;
    engineParameters_["WindowResizable"] = true;
    engineParameters_["WindowWidth"] = GetSubsystem<Graphics>()->GetDesktopResolution().x_;
    engineParameters_["WindowHeight"] = GetSubsystem<Graphics>()->GetDesktopResolution().y_;
    engineParameters_["ResourcePackages"] = "Engine.pak";

    ParseArguments();

    GetSubsystem<ResourceCache>()->SetAutoReloadResources(true);
    GetSubsystem<Input>()->SetMouseVisible(true);
}

void Editor::Start()
{
    // Create root scene node
    m_Scene->CreateScene();
    // Load editor settings and preferences
    LoadConfig();
    // Create user interface for the editor
    CreateUI();
    // Create root UI element where all 'editable' UI elements would be parented to
    CreateRootUIElement();
    // Load the initial scene if provided
}

void Editor::Stop()
{
    SaveConfig();
}

void Editor::ParseArguments()
{
    Vector<String> arguments = GetArguments();

    foreach(String argument, arguments)
    {
        if (argument.Compare("-production"))
        {
            GetSubsystem<Settings>()->SetSetting("production", true);
        }
    }
}

void Editor::HandlerUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    UpdateView(timeStep);
    UpdateStats(timeStep);
    m_Scene->UpdateScene(timeStep);
    UpdateTestAnimation(timeStep);
    UpdateGizmo();
    UpdateDirtyUI();
}

void Editor::SaveConfig()
{
    Renderer* renderer = GetSubsystem<Renderer>();
    Engine* engine = GetSubsystem<Engine>();
    Graphics* graphics = GetSubsystem<Graphics>();

    XMLFile config(context_);
    XMLElement configElem = config.CreateRoot("configuration");
    XMLElement cameraElem = configElem.CreateChild("camera");
    XMLElement objectElem = configElem.CreateChild("object");
    XMLElement renderingElem = configElem.CreateChild("rendering");
    XMLElement uiElem = configElem.CreateChild("ui");
    XMLElement hierarchyElem = configElem.CreateChild("hierarchy");
    XMLElement inspectorElem = configElem.CreateChild("attributeinspector");
    XMLElement viewElem = configElem.CreateChild("view");

    // The save config may be called on error exit so some of the objects below could still be null
    if (camera !is null)
    {
        cameraElem.SetFloat("nearclip", camera.nearClip);
        cameraElem.SetFloat("farclip", camera.farClip);
        cameraElem.SetFloat("fov", camera.fov);
        cameraElem.SetFloat("speed", cameraBaseSpeed);
        cameraElem.SetBool("limitrotation", limitRotation);
    }

    objectElem.SetFloat("newnodedistance", newNodeDistance);
    objectElem.SetFloat("movestep", moveStep);
    objectElem.SetFloat("rotatestep", rotateStep);
    objectElem.SetFloat("scalestep", scaleStep);
    objectElem.SetBool("movesnap", moveSnap);
    objectElem.SetBool("rotatesnap", rotateSnap);
    objectElem.SetBool("scalesnap", scaleSnap);
    objectElem.SetBool("applymateriallist", applyMaterialList);
    objectElem.SetAttribute("importoptions", importOptions);
    objectElem.SetInt("pickmode", pickMode);

    if (renderer)
    {
        renderingElem.SetInt("texturequality", renderer->GetTextureQuality());
        renderingElem.SetInt("materialquality", renderer->GetMaterialQuality());
        renderingElem.SetInt("shadowresolution", GetShadowResolution());
        renderingElem.SetInt("shadowquality", renderer->GetShadowQuality());
        renderingElem.SetInt("maxoccludertriangles", renderer->GetMaxOccluderTriangles());
        renderingElem.SetBool("specularlighting", renderer->GetSpecularLighting());
    }

    if (graphics)
        renderingElem.SetBool("dynamicinstancing", graphics->GetSM3Support() ? renderer->GetDynamicInstancing() : instancingSetting);

    renderingElem.SetBool("framelimiter", engine->GetMaxFps() > 0);

    uiElem.SetFloat("minopacity", uiMinOpacity);
    uiElem.SetFloat("maxopacity", uiMaxOpacity);

    hierarchyElem.SetBool("showinternaluielement", showInternalUIElement);
    hierarchyElem.SetBool("showtemporaryobject", showTemporaryObject);
    inspectorElem.SetColor("nodecolor", nodeTextColor);
    inspectorElem.SetColor("componentcolor", componentTextColor);

    inspectorElem.SetColor("originalcolor", normalTextColor);
    inspectorElem.SetColor("modifiedcolor", modifiedTextColor);
    inspectorElem.SetColor("noneditablecolor", nonEditableTextColor);
    inspectorElem.SetBool("shownoneditable", showNonEditableAttribute);

    viewElem.SetBool("showgrid", showGrid);
    viewElem.SetBool("grid2dmode", grid2DMode);
    viewElem.SetColor("defaultzoneambientcolor", renderer->GetDefaultZone()->GetAmbientColor());
    viewElem.SetColor("defaultzonefogcolor", renderer->GetDefaultZone()->GetFogColor());
    viewElem.SetFloat("defaultzonefogstart", renderer->GetDefaultZone()->GetFogStart());
    viewElem.SetFloat("defaultzonefogend", renderer->GetDefaultZone()->GetFogEnd());
    viewElem.SetInt("gridsize", gridSize);
    viewElem.SetInt("gridsubdivisions", gridSubdivisions);
    viewElem.SetFloat("gridscale", gridScale);
    viewElem.SetColor("gridcolor", gridColor);
    viewElem.SetColor("gridsubdivisioncolor", gridSubdivisionColor);

    config.Save(File(context_, m_ConfigFile, FILE_WRITE));
}

void Editor::LoadConfig()
{
    Renderer* renderer = GetSubsystem<Renderer>();
    Engine* engine = GetSubsystem<Engine>();

    if (!GetSubsystem<FileSystem>()->FileExists(m_ConfigFile))
        return;

    XMLFile config(context_);
    config.Load(File(context_, m_ConfigFile));

    XMLElement configElem = config.GetRoot();
    if (configElem.IsNull())
        return;

    XMLElement cameraElem = configElem.GetChild("camera");
    XMLElement objectElem = configElem.GetChild("object");
    XMLElement renderingElem = configElem.GetChild("rendering");
    XMLElement uiElem = configElem.GetChild("ui");
    XMLElement hierarchyElem = configElem.GetChild("hierarchy");
    XMLElement inspectorElem = configElem.GetChild("attributeinspector");
    XMLElement viewElem = configElem.GetChild("view");
    XMLElement resourcesElem = configElem.GetChild("resources");

    if (!cameraElem.IsNull())
    {
        if (cameraElem.HasAttribute("nearclip")) camera.nearClip = cameraElem.GetFloat("nearclip");
        if (cameraElem.HasAttribute("farclip")) camera.farClip = cameraElem.GetFloat("farclip");
        if (cameraElem.HasAttribute("fov")) camera.fov = cameraElem.GetFloat("fov");
        if (cameraElem.HasAttribute("speed")) cameraBaseSpeed = cameraElem.GetFloat("speed");
        if (cameraElem.HasAttribute("limitrotation")) limitRotation = cameraElem.GetBool("limitrotation");
    }

    if (!objectElem.IsNull())
    {
        if (objectElem.HasAttribute("newnodedistance")) newNodeDistance = objectElem.GetFloat("newnodedistance");
        if (objectElem.HasAttribute("movestep")) moveStep = objectElem.GetFloat("movestep");
        if (objectElem.HasAttribute("rotatestep")) rotateStep = objectElem.GetFloat("rotatestep");
        if (objectElem.HasAttribute("scalestep")) scaleStep = objectElem.GetFloat("scalestep");
        if (objectElem.HasAttribute("movesnap")) moveSnap = objectElem.GetBool("movesnap");
        if (objectElem.HasAttribute("rotatesnap")) rotateSnap = objectElem.GetBool("rotatesnap");
        if (objectElem.HasAttribute("scalesnap")) scaleSnap = objectElem.GetBool("scalesnap");
        if (objectElem.HasAttribute("applymateriallist")) applyMaterialList = objectElem.GetBool("applymateriallist");
        if (objectElem.HasAttribute("importoptions")) importOptions = objectElem.GetAttribute("importoptions");
        if (objectElem.HasAttribute("pickmode")) pickMode = objectElem.GetInt("pickmode");
    }

    if (!renderingElem.IsNull())
    {
        if (renderingElem.HasAttribute("texturequality")) renderer->SetTextureQuality(renderingElem.GetInt("texturequality"));
        if (renderingElem.HasAttribute("materialquality")) renderer->SetMaterialQuality(renderingElem.GetInt("materialquality"));
        if (renderingElem.HasAttribute("shadowresolution")) SetShadowResolution(renderingElem.GetInt("shadowresolution"));
        if (renderingElem.HasAttribute("shadowquality")) renderer->SetShadowQuality(renderingElem.GetInt("shadowquality"));
        if (renderingElem.HasAttribute("maxoccludertriangles")) renderer->SetMaxOccluderTriangles(renderingElem.GetInt("maxoccludertriangles"));
        if (renderingElem.HasAttribute("specularlighting")) renderer->SetSpecularLighting(renderingElem.GetBool("specularlighting"));
        if (renderingElem.HasAttribute("dynamicinstancing")) renderer->SetDynamicInstancing(instancingSetting = renderingElem.GetBool("dynamicinstancing"));
        if (renderingElem.HasAttribute("framelimiter")) engine->SetMaxFps(renderingElem.GetBool("framelimiter") ? 200 : 0);
    }

    if (!uiElem.IsNull())
    {
        if (uiElem.HasAttribute("minopacity")) uiMinOpacity = uiElem.GetFloat("minopacity");
        if (uiElem.HasAttribute("maxopacity")) uiMaxOpacity = uiElem.GetFloat("maxopacity");
    }

    if (!hierarchyElem.IsNull())
    {
        if (hierarchyElem.HasAttribute("showinternaluielement")) showInternalUIElement = hierarchyElem.GetBool("showinternaluielement");
        if (hierarchyElem.HasAttribute("showtemporaryobject")) showTemporaryObject = hierarchyElem.GetBool("showtemporaryobject");
        if (inspectorElem.HasAttribute("nodecolor")) nodeTextColor = inspectorElem.GetColor("nodecolor");
        if (inspectorElem.HasAttribute("componentcolor")) componentTextColor = inspectorElem.GetColor("componentcolor");
    }

    if (!inspectorElem.IsNull())
    {
        if (inspectorElem.HasAttribute("originalcolor")) normalTextColor = inspectorElem.GetColor("originalcolor");
        if (inspectorElem.HasAttribute("modifiedcolor")) modifiedTextColor = inspectorElem.GetColor("modifiedcolor");
        if (inspectorElem.HasAttribute("noneditablecolor")) nonEditableTextColor = inspectorElem.GetColor("noneditablecolor");
        if (inspectorElem.HasAttribute("shownoneditable")) showNonEditableAttribute = inspectorElem.GetBool("shownoneditable");
    }

    if (!viewElem.IsNull())
    {
        if (viewElem.HasAttribute("defaultzoneambientcolor")) renderer->GetDefaultZone()->SetAmbientColor(viewElem.GetColor("defaultzoneambientcolor"));
        if (viewElem.HasAttribute("defaultzonefogcolor")) renderer->GetDefaultZone()->SetFogColor(viewElem.GetColor("defaultzonefogcolor"));
        if (viewElem.HasAttribute("defaultzonefogstart")) renderer->GetDefaultZone()->SetFogStart(viewElem.GetInt("defaultzonefogstart"));
        if (viewElem.HasAttribute("defaultzonefogend")) renderer->GetDefaultZone()->SetFogEnd(viewElem.GetInt("defaultzonefogend"));
        if (viewElem.HasAttribute("showgrid")) showGrid = viewElem.GetBool("showgrid");
        if (viewElem.HasAttribute("grid2dmode")) grid2DMode = viewElem.GetBool("grid2dmode");
        if (viewElem.HasAttribute("gridsize")) gridSize = viewElem.GetInt("gridsize");
        if (viewElem.HasAttribute("gridsubdivisions")) gridSubdivisions = viewElem.GetInt("gridsubdivisions");
        if (viewElem.HasAttribute("gridscale")) gridScale = viewElem.GetFloat("gridscale");
        if (viewElem.HasAttribute("gridcolor")) gridColor = viewElem.GetColor("gridcolor");
        if (viewElem.HasAttribute("gridsubdivisioncolor")) gridSubdivisionColor = viewElem.GetColor("gridsubdivisioncolor");
    }
}

DEFINE_APPLICATION_MAIN(Editor)