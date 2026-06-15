// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "Components/TintedBillboardComponent.h"
#include "EdIconComponent.generated.h"

#define DECLARE_ICON_COMPONENT TObjectPtr<UEdIconComponent> EdIconComp = nullptr;
#define DEFINE_ICON_COMPONENT \
	EdIconComp = CreateEditorOnlyDefaultSubobject<UEdIconComponent>(TEXT("EdIcon")); \
	if (EdIconComp) EdIconComp->SetupAttachment(GetRootComponent());

#define UPDATE_ICON_MAX_COMP(Max) if (EdIconComp) EdIconComp->SetMaxComponents(Max);
#define UPDATE_ICON_SIZE(Size) if (EdIconComp) EdIconComp->UpdateSize(Size);
#define UPDATE_ICON_IMAGE(Image) if (EdIconComp) EdIconComp->UpdateIcon(TEXT(Image));

/**
 * An intelligent Billboard Component designed to represent "Logic Actors" in the editor.
 * This icon will automatically hide itself if the Owning Actor's component count 
 * exceeds 'MaxComps' (default: 1). This prevents icon clutter on actors that already have 
 * visible 3D meshes or complex components.
 * 
 * Note: All visualization data is stripped in non-editor builds.
 */
UCLASS(MinimalAPI, NotBlueprintable, NotBlueprintType)
class UEdIconComponent final : public UTintedBillboardComponent
{
	GENERATED_BODY()

public:

	UEdIconComponent();

	TOROCORE_API void SetMaxComponents(const uint8 Max) { MaxComps = Max; }
	TOROCORE_API void UpdateSize(const float Scale = 0.5f) { SetWorldScale3D(FVector(Scale)); }
	TOROCORE_API void UpdateIcon(const FString& TexturePath = TEXT("/Engine/EditorResources/EmptyActor.EmptyActor"))
	{
		SetSprite(LoadObject<UTexture2D>(nullptr, *TexturePath));
	}

private:

	uint8 MaxComps = 1;

#if WITH_EDITOR
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
#endif
};
