// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "WorldActions/WorldActionActor.h"
#include "ActorActions.generated.h"

UCLASS(NotBlueprintable, DisplayName = "[Actor] Change Visibility")
class TORORUNTIME_API UWorldAction_Visibility final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Visibility()
		: Type(EWorldActionApplyType::Set), bVisible(false)
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		EWorldActionApplyType Type;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Type == EWorldActionApplyType::Set", EditConditionHides))
		bool bVisible;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Actor] Enable Collision")
class TORORUNTIME_API UWorldAction_Collision final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Collision()
		: Type(EWorldActionApplyType::Set), bCollisionEnabled(false)
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		EWorldActionApplyType Type;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Type == EWorldActionApplyType::Set", EditConditionHides))
		bool bCollisionEnabled;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Actor] Enabled State")
class TORORUNTIME_API UWorldAction_ActorEnabled final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_ActorEnabled()
		: Type(EWorldActionApplyType::Set), bEnabled(false)
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		EWorldActionApplyType Type;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Type == EWorldActionApplyType::Set", EditConditionHides))
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Action, meta = (MustImplement = "/Script/ToroCore.EnabledState"))
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Actor] Apply Tags")
class TORORUNTIME_API UWorldAction_ActorTags final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_ActorTags() {}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FName> AddTags;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FName> RemoveTags;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Actor] Primitive Data")
class TORORUNTIME_API UWorldAction_PrimitiveData final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_PrimitiveData() 
		: bOverrideDefault(true)
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		bool bOverrideDefault;

	UPROPERTY(EditAnywhere, Category = Action)
		TMap<uint8, float> PrimitiveData;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	virtual void OnExecute_Implementation() override;
};