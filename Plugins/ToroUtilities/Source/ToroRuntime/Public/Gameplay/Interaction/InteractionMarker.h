// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/ToroInterpolator.h"
#include "Components/BillboardComponent.h"
#include "InteractionMarker.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInteractionMarker final : public UBillboardComponent
{
	GENERATED_BODY()

public:

	UInteractionMarker();

	UPROPERTY(EditAnywhere, Category = Settings)
		float MaxDistance;

	UPROPERTY(EditAnywhere, Category = Settings)
		float ScaleSpeed;

private:

	bool bTargetVis;
	float SlowInterval;
	FVector CachedBaseSize;
	FToroInterpVector SizeInterp;
	TObjectPtr<APlayerCameraManager> CamManager;

	void UpdateTargetSize();
	FVector GetCameraPos() const;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
