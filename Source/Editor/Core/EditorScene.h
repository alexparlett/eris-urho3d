////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <Ptr.h>
#include <Scene.h>
#include <Vector.h>
#include <Node.h>
#include <Component.h>
#include <XMLFile.h>

const int PICK_GEOMETRIES = 0;
const int PICK_LIGHTS = 1;
const int PICK_ZONES = 2;
const int PICK_RIGIDBODIES = 3;
const int PICK_UI_ELEMENTS = 4;
const int MAX_PICK_MODES = 5;
const int MAX_UNDOSTACK_SIZE = 256;

class EditorScene : public Urho3D::Object
{
    OBJECT(EditorScene)

public:
    EditorScene(Urho3D::Context* context);

    void CreateScene();
    bool LoadScene(const Urho3D::String& fileName);
    bool SaveSceneAs(const Urho3D::String& fileName);
    bool SaveScene();

    void StartSceneUpdate();
    void UpdateScene(float timestep);
    bool ToogleSceneUpdate();

    void CreateNode(Urho3D::CreateMode mode);
    template<typename T> void CreateComponent();

    bool LoadNode(const Urho3D::String& fileName);
    bool SaveNode(const Urho3D::String& fileName);

    bool LoadParticleData(const Urho3D::String& fileName);
    bool SaveParticleData(const Urho3D::String& fileName);

    bool SceneDelete();
    bool SceneCut();
    bool SceneCopy();
    bool ScenePaste();
    bool SceneUnparent();
    bool SceneChangeParent(Urho3D::Node* source, Urho3D::Node* target, bool createUndoAction = true);
    bool SceneChangeParent(Urho3D::Node* source, Urho3D::Vector<Urho3D::Node*> sourceNodes, Urho3D::Node* target, bool createUndoAction = true);
    bool SceneToggleEnable();
    bool SceneResetRotation();
    bool SceneResetScale();
    bool SceneResetToDefault();
    bool SceneSelectAll();
    bool SceneRebuildNavigation();

private:
    void SetSceneModified();
    void ClearSceneSelection();
    bool ResetScene();

    Urho3D::Scene* m_Scene;
    Urho3D::Vector<Urho3D::XMLFile> m_SceneCopyBuffer;
    Urho3D::String m_InstantiateFileName;
    Urho3D::CreateMode m_InstantiateMode;
    bool m_SceneModified;
    bool m_RunUpdate;
    bool m_SuppressSceneChanges;
    bool m_InSelectionModify;
    Urho3D::Vector<Urho3D::Node*> m_SelectedNodes;
    Urho3D::Vector<Urho3D::Node*> m_SelectedComponents;
    Urho3D::Node* m_EditNode;
    Urho3D::Vector<Urho3D::Node*> m_EditNodes;
    Urho3D::Vector<Urho3D::Component*> m_EditComponents;
    unsigned int m_NumEditableComponentsPerNode;
    Urho3D::Vector<EditActionGroup> m_UndoStack;
    unsigned int m_UndoStackPos;
};

