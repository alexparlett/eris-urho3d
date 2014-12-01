class EscapeMenu
{
    Window@ window;

    EscapeMenu(UIElement@ uiRoot)
    {
        @window = uiRoot.CreateChild("Window", "EscapeMenuWindow");
        window.SetStyleAuto();
        window.SetAlignment(HA_CENTER, VA_CENTER);
        window.SetMinSize(195, 0);
        window.layoutBorder = IntRect(15, 15, 15, 15);
        window.layoutMode = LM_VERTICAL;
        window.layoutSpacing = 5;
        window.visible = false;

        Font@ font = cache.GetResource("Font", "Fonts/CaviarDreams.ttf");

        Button@ continueBtn = CreateMenuButton(font, locale.Localize(0, 6));
        Button@ loadBtn = CreateMenuButton(font, locale.Localize(0, 2));
        Button@ saveBtn = CreateMenuButton(font, locale.Localize(0, 7));
        Button@ optionsBtn = CreateMenuButton(font, locale.Localize(0, 4));
        Button@ exitBtn = CreateMenuButton(font, locale.Localize(0, 5));

        window.AddChild(continueBtn);
        window.AddChild(loadBtn);
        window.AddChild(saveBtn);
        window.AddChild(optionsBtn);
        window.AddChild(exitBtn);

        SubscribeToEvent(continueBtn, "Released", "HandleContinue");
        SubscribeToEvent(loadBtn, "Released", "HandleLoad");
        SubscribeToEvent(saveBtn, "Released", "HandleSave");
        SubscribeToEvent(optionsBtn, "Released", "HandleOptions");
        SubscribeToEvent(exitBtn, "Released", "HandleExit");
        SubscribeToEvent("KeyDown", "HandleToggle");
    }

    void ToggleVisible()
    {
        if (window.visible)
            window.visible = false;
        else
            window.visible = true;
    }

    Button@ CreateMenuButton(Font@ font, const String& context)
    {
        Button@ button = Button();
        button.SetStyleAuto();
        button.focusMode = FM_RESETFOCUS;
        button.layoutMode = LM_HORIZONTAL;
        button.minHeight = 30;
        button.layoutBorder = IntRect(10, 10, 10, 10);

        Text@ text = Text();
        text.SetFont(font, 12);
        text.SetAlignment(HA_CENTER, VA_CENTER);
        text.text = context;
        text.textAlignment = HA_CENTER;

        button.AddChild(text);

        return button;
    }

    void HandleContinue(StringHash eventType, VariantMap& eventData)
    {
        ToggleVisible();
    }

    void HandleLoad(StringHash eventType, VariantMap& eventData)
    {
    }

    void HandleSave(StringHash eventType, VariantMap& eventData)
    {
    }

    void HandleOptions(StringHash eventType, VariantMap& eventData)
    {
    }

    void HandleExit(StringHash eventType, VariantMap& eventData)
    {
        SendEvent("GameFinished");
    }

    void HandleToggle(StringHash eventType, VariantMap& eventData)
    {
        int scanCode = eventData["Scancode"].GetInt();
        int qualifiers = eventData["Qualifiers"].GetInt();

        if (scanCode == SCANCODE_ESCAPE && (!GetSubsystem<UI>()->GetFocusElement() || GetSubsystem<UI>->GetFocusElement() == window))
        {
            ToggleVisible();
        }
}