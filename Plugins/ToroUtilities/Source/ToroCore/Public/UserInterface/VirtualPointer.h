// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/Border.h"
#include "VirtualPointer.generated.h"

UCLASS()
class TOROCORE_API UVirtualPointer final : public UBorder
{
	GENERATED_BODY()

public:

	UVirtualPointer();
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = VirtualMouse)
		void SimulateClick() const;

	UFUNCTION(BlueprintCallable, Category = VirtualMouse)
		FVector2D GetPosition() const { return GetCachedGeometry().GetAbsolutePosition(); }

#if WITH_EDITOR
protected:
	virtual void SynchronizeProperties() override;
	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("UMG", "Input", "Input"); }
#endif
};
