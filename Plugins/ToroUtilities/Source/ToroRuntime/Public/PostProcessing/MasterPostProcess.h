// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PostProcessTypes.h"
#include "Actors/ToroActor.h"
#include "MiscObjects/UDSSetterObject.h"
#include "UserSettings/ToroUserSettings.h"
#include "MasterPostProcess.generated.h"

UCLASS(NotBlueprintable, HideCategories = (HLOD, Collision, Physics), PrioritizeCategories = (Settings, PostProcessing))
class TORORUNTIME_API AMasterPostProcess final : public AToroActor
{
	GENERATED_BODY()

public:

	AMasterPostProcess();

	static AMasterPostProcess* Get(const UObject* ContextObject, const bool bCreateIfNull = true);

	UFUNCTION(BlueprintPure, Category = PostProcess)
		UMaterialInstanceDynamic* FindBlendable(UMaterialInterface* InParent);

	UFUNCTION(BlueprintCallable, Category = PostProcess)
		UMaterialInstanceDynamic* FindOrAddBlendable(UMaterialInterface* InParent);

	UFUNCTION(BlueprintCallable, Category = PostProcess)
		void RemoveBlendable(UMaterialInterface* InParent);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = PostProcess, DisplayName = "Set UDS Settings")
		void SetUDSSettings(const FUDSSettings& InSettings) const;

	UFUNCTION(BlueprintCallable, Category = PostProcess)
		void ApplySettings();

	UFUNCTION(BlueprintPure, Category = PostProcess)
		bool HasLumenGI() const;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<class UPostProcessComponent> PostProcess;

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, Category = Settings)
		FPPBloomChoice BloomOverride;

	/* Advanced bloom control for game settings */
	UPROPERTY(EditAnywhere, Category = Settings)
		FPPMotionBlurChoice MotionBlurOverride;

	/* Advanced lumen control for game settings */
	UPROPERTY(EditAnywhere, Category = Settings)
		FPPLumenChoice LumenOverride;

	UPROPERTY(EditAnywhere, Category = PostProcessing, meta = (ShowOnlyInnerProperties))
		FPostProcessSettings Settings;
	
	UPROPERTY(Transient)
		TObjectPtr<UUDSSetterObject> UDSSetterObj;
	
	UPROPERTY(Transient)
		TSet<TObjectPtr<UMaterialInstanceDynamic>> Blendables;

	// Weak Ptr since Blendables hold the actual instance
	TWeakObjectPtr<UMaterialInstanceDynamic> BrightnessMID;

	void OnSettingsUpdated(const ESettingApplyType Type);

	virtual void BeginPlay() override;
	virtual void EnableStateChanged(const bool bState) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	bool MultiActorCheck() const;
#endif
};
