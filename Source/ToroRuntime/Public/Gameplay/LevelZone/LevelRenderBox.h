// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "Components/CullingComponent.h"
#include "LevelRenderBox.generated.h"

UCLASS()
class TORORUNTIME_API ALevelRenderBox : public AToroActor
{
	GENERATED_BODY()

public:

	ALevelRenderBox();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UStaticMeshComponent> CubeComponent;

protected:

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UMaterialInterface> ClearMaterial;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bInvert;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<TSoftObjectPtr<AActor>> Targets;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools)
		FName FindTag = "Cullable";
#endif
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools)
		void FindTargets();
#endif
	
	TSet<TWeakObjectPtr<UCullingComponent>> CullComponents;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
