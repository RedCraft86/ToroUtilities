// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroCameraManager.generated.h"

UCLASS()
class TORORUNTIME_API AToroCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AToroCameraManager();
	
	PLAYER_CLASS_GETTER(AToroCameraManager, GetPlayerCameraManager)

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
