// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActionBase.h"
#include "Components/ActorComponent.h"
#include "WorldActionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (World), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UWorldActionManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UWorldActionManager();

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bAutoConstruction;

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void SetActions(UPARAM(ref) FWorldActionArray& InActions, bool bRunConstruction);

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void AppendActions(UPARAM(ref) FWorldActionArray& InActions, bool bRunConstruction);

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void RunActions();

	void RunConstruction();

private:

	// POTENTIALLY DANGEROUS due to possible dangling pointers
	// If anything sus happens, this might need a revisit
	TArray<FWorldActionBase*> ActionPtrs;

	void ForEachAction(const TFunction<void(FWorldActionBase*)>& Func);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
