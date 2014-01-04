UIElement@ bottomBar;

Text@ editorModeText;
Text@ renderStatsText;
Text@ cameraPosText;

void CreateStatsBar()
{
    bottomBar = BorderImage("BottomBar");
    bottomBar.style = "EditorMenuBar";
    bottomBar.SetFixedSize(graphics.width, 30);   
    bottomBar.opacity = uiMaxOpacity;
    bottomBar.SetPosition(0, ui.root.height - bottomBar.height);
    ui.root.AddChild(bottomBar);

    Font@ font = cache.GetResource("Font", "Fonts/Anonymous Pro.ttf");

    editorModeText = Text();
    bottomBar.AddChild(editorModeText);
    
    cameraPosText = Text();
    bottomBar.AddChild(cameraPosText);   
    
    renderStatsText = Text();
    bottomBar.AddChild(renderStatsText);
    
    int middle = bottomBar.width / 2;
    
    SetupStatsBarText(editorModeText, font, HA_LEFT, VA_CENTER);
    SetupStatsBarText(cameraPosText, font, HA_LEFT, VA_CENTER);    
    SetupStatsBarText(renderStatsText, font, HA_LEFT, VA_CENTER);   
}

void SetupStatsBarText(Text@ text, Font@ font, HorizontalAlignment hAlign, VerticalAlignment vAlign)
{
    text.horizontalAlignment = hAlign;
    text.verticalAlignment = vAlign;
    text.SetFont(font, 11);
    text.color = Color(1, 1, 0);
    text.priority = -100;
}

void UpdateStatsPosition()
{
    bottomBar.SetFixedWidth(graphics.width);
    bottomBar.SetPosition(0, ui.root.height - bottomBar.height);
  
    int middle = bottomBar.width / 2;
  
    editorModeText.position = IntVector2(4,0);
    cameraPosText.position = IntVector2(middle - (cameraPosText.width / 2),0);
    renderStatsText.position = IntVector2(bottomBar.width - renderStatsText.width,0);
}

void UpdateStats(float timeStep)
{
    editorModeText.text = String(
        "Mode: " + editModeText[editMode] +
        "  Axis: " + axisModeText[axisMode] +
        "  Pick: " + pickModeText[pickMode] +
        "  Fill: " + fillModeText[fillMode] +
        "  Updates: " + (runUpdate ? "Running" : "Paused"));

    renderStatsText.text = String(
        "Tris: " + renderer.numPrimitives +
        "  Batches: " + renderer.numBatches +
        "  Lights: " + renderer.numLights[true] +
        "  Shadowmaps: " + renderer.numShadowMaps[true] +
        "  Occluders: " + renderer.numOccluders[true]);

    Vector3 cameraPos = cameraNode.position;
    String xText(cameraPos.x);
    String yText(cameraPos.y);
    String zText(cameraPos.z);
    xText.Resize(8);
    yText.Resize(8);
    zText.Resize(8);

    cameraPosText.text = String(
        "Pos: " + xText + " " + yText + " " + zText);
        
    editorModeText.size = editorModeText.minSize;
    renderStatsText.size = renderStatsText.minSize;
    cameraPosText.size = cameraPosText.minSize; 

    UpdateStatsPosition();
}