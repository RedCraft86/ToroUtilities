// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/BillboardComponent.h"
#include "EditorVisualComponent.generated.h"

/**
 * #if WITH_EDITOR
 * DECLARE_VISUAL_COMPONENT
 * #endif
 */
#define DECLARE_VISUAL_COMPONENT TObjectPtr<UEditorVisualComponent> EdVisualComp = nullptr;

/**
 * #if WITH_EDITOR
 * DEFINE_VISUAL_COMPONENT
 * #endif
 */
#define DEFINE_VISUAL_COMPONENT \
	if (!FApp::IsGame()) EdVisualComp = CreateEditorOnlyDefaultSubobject<UEditorVisualComponent>("EdVisual"); \
	if (EdVisualComp) EdVisualComp->SetupAttachment(GetRootComponent());

#define UPDATE_VISUAL_MAX_COMP(Max) if (EdVisualComp) EdVisualComp->SetMaxComponents(Max);
#define UPDATE_VISUAL_SIZE(Size) if (EdVisualComp) EdVisualComp->UpdateSize(Size);
#define UPDATE_VISUAL_ICON(Icon) if (EdVisualComp) EdVisualComp->UpdateIcon(Icon);

UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType)
class UEditorVisualComponent final : public UBillboardComponent
{
    GENERATED_BODY()

public:
    TOROCORE_API UEditorVisualComponent()
    {
        PrimaryComponentTick.bCanEverTick = true;
	    PrimaryComponentTick.TickGroup = TG_DuringPhysics;
        PrimaryComponentTick.TickInterval = 0.2f;
        bIsScreenSizeScaled = true;
        bAutoActivate = false;
        bIsEditorOnly = true;
        bTickInEditor = true;
        SetHiddenInGame(true);
#if WITH_EDITORONLY_DATA
        SetIsVisualizationComponent(true);
#endif
        UpdateSize();
        UpdateIcon();
    }

#if WITH_EDITOR
    TOROCORE_API void SetMaxComponents(const uint8 Max) { MaxComps = Max; }
    TOROCORE_API void UpdateSize(const float Scale = 0.5f) { SetWorldScale3D(FVector(Scale)); }
    TOROCORE_API void UpdateIcon(const FString& TexturePath = TEXT("/Engine/EditorResources/EmptyActor.EmptyActor"))
    {
        SetSprite(LoadObject<UTexture2D>(nullptr, *TexturePath));
    }

private:
    uint8 MaxComps = 1;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override
    {
        Super::TickComponent(DeltaTime, TickType, TickFunc);
        uint8 Count = 0;
        TArray<USceneComponent*> Components;
        GetOwner()->GetComponents<USceneComponent>(Components);
        for (const USceneComponent* Comp : Components)
        {
            if (Comp && Comp != this)
            {
                if (++Count > MaxComps) break;
            }
        }
        
        SetVisibility(MaxComps > 0 && Count <= MaxComps);
    }
#endif

    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        DestroyComponent();
    }
};