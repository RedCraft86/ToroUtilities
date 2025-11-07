// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogToroGraphEd, All, All);

class FToroGraphEdModule final : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

    TSharedPtr<struct FGraphPanelNodeFactory> DataGraphNodeFactory;
};

class FToroGraphEdStyle final
{
public:

    static void Init();
    static void Shutdown();
    static const ISlateStyle& Get() { return *StyleSet; }
    static const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }

private:

    static TSharedPtr<FSlateStyleSet> StyleSet;
};

namespace ToroGraphStatics
{
    static constexpr FLinearColor InvalidBgColor(0.4f, 0.0f, 0.0f);
    static constexpr FLinearColor DefaultPinColor(0.02f, 0.02f, 0.02f);

    static const FName EditorAppName(TEXT("ToroGraphEditorApp"));
    static const FName PropertyID(TEXT("Property"));
    static const FName ViewportID(TEXT("Viewport"));
}
