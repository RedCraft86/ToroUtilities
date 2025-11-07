// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Gameplay/WorldActions/WorldActionBase.h"
#include "ActorActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Actor", meta = (Hidden))
struct TORORUNTIME_API FWATask_Actors : public FWorldActionBase
{
	GENERATED_BODY()

protected:

	static void ValidateTargets(TSet<TSoftObjectPtr<AActor>>& InTargets);
};

USTRUCT(BlueprintType, DisplayName = "Visibility")
struct TORORUNTIME_API FWTActor_Visibility final : public FWATask_Actors
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Action)
		bool bToggle;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "!bToggle", EditConditionHides))
		bool bVisible;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWTActor_Visibility(): bToggle(false), bVisible(true) {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Collision")
struct TORORUNTIME_API FWTActor_Collision final : public FWATask_Actors
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Action)
		bool bToggle;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "!bToggle", EditConditionHides))
		bool bHasCollision;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWTActor_Collision(): bToggle(false), bHasCollision(true) {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Tags")
struct TORORUNTIME_API FWTActor_Tags final : public FWATask_Actors
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FName> Add;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FName> Remove;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWTActor_Tags() {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Enabled")
struct TORORUNTIME_API FWTActor_Enabled final : public FWATask_Actors
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Action)
		bool bToggle;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "!bToggle", EditConditionHides))
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Action, meta = (MustImplement = "/Script/ToroCore.ActorStateInterface"))
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWTActor_Enabled(): bToggle(false), bEnabled(true) {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Static Mesh", meta = (Hidden))
struct TORORUNTIME_API FWATask_StaticMeshes : public FWATask_Actors
{
	GENERATED_BODY()

protected:

	static void ValidateTargets(TSet<TSoftObjectPtr<AStaticMeshActor>>& InTargets);
};

USTRUCT(BlueprintType, DisplayName = "Materials")
struct TORORUNTIME_API FWTActor_Materials final : public FWATask_StaticMeshes
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Action)
		TMap<uint8, TSoftObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;
	
	FWTActor_Materials() {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Primitive Data")
struct TORORUNTIME_API FWTActor_PrimitiveData final : public FWATask_StaticMeshes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		bool bOverrideDefault;

	UPROPERTY(EditAnywhere, Category = Action)
		TMap<uint8, float> Data;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;
	
	FWTActor_PrimitiveData(): bOverrideDefault(true) {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};