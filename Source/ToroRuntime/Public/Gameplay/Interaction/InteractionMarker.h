// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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

	UPROPERTY(EditAnywhere, Category = Settings, meta = (UIMin = 1.0f, AllowPreserveRatio = true))
		FVector BaseSize;

	void ResetScale();

private:

	bool bTargetVis;
	TObjectPtr<APlayerCameraManager> CamManager;

	FVector GetCameraPos() const;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
