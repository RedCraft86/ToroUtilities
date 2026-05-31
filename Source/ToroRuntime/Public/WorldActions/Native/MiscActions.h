// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "LevelSequenceActor.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

UCLASS(NotBlueprintable, DisplayName = "[Misc] Level Streaming")
class TORORUNTIME_API UWorldAction_LevelStreaming final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_LevelStreaming() {}

private:

	/* (Level, MakeVisibleAfterLoad) */
	UPROPERTY(EditAnywhere, Category = Action)
		TMap<TSoftObjectPtr<UWorld>, bool> Load;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<UWorld>> Unload;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Misc] Level Sequence")
class TORORUNTIME_API UWorldAction_LevelSequence final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_LevelSequence() {}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<ALevelSequenceActor> Sequence;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Misc] Remote Event")
class TORORUNTIME_API UWorldAction_RemoteEvent final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_RemoteEvent()
		: EventName(NAME_None) 
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		FName EventName;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Misc] Global Event")
class TORORUNTIME_API UWorldAction_GlobalEvent final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_GlobalEvent()
		: Instigator(nullptr) 
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "Event"))
		FGameplayTag Identifier;

	/* If nullptr, world context will be cast to actor instead. */
	UPROPERTY(EditAnywhere, Category = Action, DisplayName = "Instigator (Optional)")
		TSoftObjectPtr<AActor> Instigator;

	UPROPERTY(EditAnywhere, Category = Action)
		FInstancedStruct Payload;

	virtual void OnExecute_Implementation() override;
};