////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

#include <Object.h>
#include <Node.h>
#include <Camera.h>
#include <CustomGeometry.h>

enum EditMode
{
    EDIT_MOVE = 0,
    EDIT_ROTATE,
    EDIT_SCALE,
    EDIT_SELECT
};

enum AxisMode
{
    AXIS_WORLD = 0,
    AXIS_LOCAL
};

enum SnapScaleMode
{
    SNAP_SCALE_FULL = 0,
    SNAP_SCALE_HALF,
    SNAP_SCALE_QUARTER
};

class EditorView : public Urho3D::Object
{
    OBJECT(EditorView)

public:
    EditorView(Urho3D::Context* context);

private:
    Urho3D::Node* m_CameraNode;
    Urho3D::Camera* m_Camera;
    Urho3D::Node* m_GridNode;
    Urho3D::CustomGeometry* m_Grid;
};

