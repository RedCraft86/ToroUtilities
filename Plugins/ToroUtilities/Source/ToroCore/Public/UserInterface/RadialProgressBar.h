// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Image.h"
#include "RadialProgressBar.generated.h"

UCLASS(PrioritizeCategories = (Appearance))
class TOROCORE_API URadialProgressBar final : public UImage
{
	GENERATED_BODY()

public:

	URadialProgressBar();
	
	UFUNCTION(BlueprintCallable, Category = ProgressBar)
		void SetProgress(const float InProgress);
	
	UFUNCTION(BlueprintPure, Category = ProgressBar)
		float GetProgress() const { return Progress; }

protected:

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (UIMin = 0.0f, UIMax = 1.0f))
		float Progress;

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (GetOptions = "ScalarParams"))
		FName Parameter;

	UPROPERTY(EditAnywhere, Category = Appearance)
		TObjectPtr<UMaterialInterface> BarMaterial;

	UPROPERTY(Transient)
		TObjectPtr<UMaterialInstanceDynamic> MatInstance;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, meta = (TransientToolProperty = true))
		TArray<FName> ScalarParams;
#endif

	void UpdateMatInstance();
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bWholeView) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override { return NSLOCTEXT("UMG", "Common", "Common"); }
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		return Super::CanEditChange(InProperty) || !InProperty || InProperty->GetFName() != "ResourceObject";
	}
#endif
};
