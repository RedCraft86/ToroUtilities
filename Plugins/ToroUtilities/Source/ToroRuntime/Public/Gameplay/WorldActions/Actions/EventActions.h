// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Gameplay/WorldActions/WorldActionBase.h"
#include "EventActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Events", meta = (Hidden))
struct TORORUNTIME_API FWATask_Events : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Remote Event")
struct TORORUNTIME_API FWTEvent_Remote final : public FWATask_Events
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		FName EventName;

	FWTEvent_Remote(): EventName(NAME_None) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Actor Event")
struct TORORUNTIME_API FWTEvent_Actor final : public FWATask_Events
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		FName EventName;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWTEvent_Actor(): EventName(NAME_None) {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Global Event")
struct TORORUNTIME_API FWTEvent_Global final : public FWATask_Events
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "GlobalEvent"))
		FGameplayTag EventID;

	UPROPERTY(EditAnywhere, Category = Action)
		bool bHasPayload;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "bHasPayload", EditConditionHides))
		TSoftObjectPtr<AActor> OptionalInvoker;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "bHasPayload", EditConditionHides))
		TMap<FName, FString> PayloadStrings;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "bHasPayload", EditConditionHides))
		TMap<FName, float> PayloadFloats;

	FWTEvent_Global(): bHasPayload(false) {}

protected:

	virtual void RunAction() override;
};