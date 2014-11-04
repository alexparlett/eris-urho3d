#include "Scripts/UI/Include/EscapeMenu.as"

class GalaxyUI : UserInterface, ScriptObject
{
    UIElement@ uiRoot;
    EscapeMenu@ escapeMenu;
    
    GalaxyUI()
    {
    }

    void Create()
    {
        uiRoot = ui.root.CreateChild("UIElement","GalaxyRoot");
        uiRoot.SetSize(graphics.width, graphics.height);
        
        @escapeMenu = EscapeMenu(uiRoot);
        
        SubscribeToEvent("GameStart", "HandleGameStarted");
        SubscribeToEvent("GameFinished", "HandleGameFinished");        
        SubscribeToEvent("ToggleEscapeMenu", "HandleEscapeMenu");   
    }
    
    void HandleGameStarted(StringHash eventType, VariantMap& eventData)
    {
        uiRoot.visible = true;
    }
    
    void HandleGameFinished(StringHash eventType, VariantMap& eventData)
    {
        uiRoot.Remove();
    }
    
    void HandleEscapeMenu(StringHash eventType, VariantMap& eventData)
    {
        escapeMenu.ToggleVisible();
    }
}