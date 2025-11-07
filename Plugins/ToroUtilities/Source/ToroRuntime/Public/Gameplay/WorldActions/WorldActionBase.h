// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "WorldActionBase.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "World Actions", meta = (Hidden))
struct TORORUNTIME_API FWorldActionBase
{
	GENERATED_BODY()

	FWorldActionBase(): bShouldTick(false) {}
	virtual ~FWorldActionBase() = default;

	virtual void RunAction() {}
	virtual void OnBeginPlay() {}
	virtual void OnConstruction() {}
	virtual void OnTick(const float DeltaTime) {}
	virtual bool ShouldTick() const { return bShouldTick; }

	const UObject* GetWorldContext() const { return WorldContext.Get(); }
	void SetWorldContext(const UObject* Obj)
	{
		if (Obj && (!WorldContext.IsValid() || Obj != WorldContext))
		{
			WorldContext = Obj;
		}
	}

protected:

	bool bShouldTick;
	TWeakObjectPtr<const UObject> WorldContext;
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FWorldActionArray final
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FWorldActionArrayDetails;
#endif

	FWorldActionArray(): bShouldTick(false) {}

	void RunAction(const UObject* Context);
	void OnBeginPlay(const UObject* Context);
	void OnConstruction(const UObject* Context);
	void OnTick(const UObject* Context, const float DeltaTime);

	bool ShouldTick() const { return bShouldTick; }
	
	TArray<FWorldActionBase*> GetPtrs();
	TArray<TInstancedStruct<FWorldActionBase>>& GetActions() { return Actions; }

private:

	UPROPERTY(EditAnywhere, Category = Actions, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	bool bShouldTick;
};
