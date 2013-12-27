////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#include "EditorScene.h"

using namespace Urho3D;

EditorScene::EditorScene(Context* context) :
Object(context),
m_Scene(NULL),
m_SceneCopyBuffer(Vector<XMLFile>()),
m_InstantiateFileName(String::EMPTY),
m_InstantiateMode(REPLICATED),
m_SceneModified(false),
m_RunUpdate(false),
m_SuppressSceneChanges(false),
m_InSelectionModify(false),
m_SelectedNodes(Vector<Node*>()),
m_SelectedComponents(Vector<Node*>()),
m_EditNode(NULL),
m_EditNodes(Vector<Node*>()),
m_EditComponents(Vector<Component*>()),
m_NumEditableComponentsPerNode(1),
m_UndoStack(Vector<EditActionGroup>()),
m_UndoStackPos(0)
{
}