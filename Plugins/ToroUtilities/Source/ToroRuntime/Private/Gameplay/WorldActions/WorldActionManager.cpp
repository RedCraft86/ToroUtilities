// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/WorldActionManager.h"

UWorldActionManager::UWorldActionManager(): bAutoConstruction(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UWorldActionManager::SetActions(FWorldActionArray& InActions, bool bRunConstruction)
{
	ActionPtrs = InActions.GetPtrs();
	if (bRunConstruction)
	{
		RunConstruction();
	}
}

void UWorldActionManager::AppendActions(FWorldActionArray& InActions, bool bRunConstruction)
{
	ActionPtrs.Append(InActions.GetPtrs());
	if (bRunConstruction)
	{
		RunConstruction();
	}
}

void UWorldActionManager::RunActions()
{
	ForEachAction([](FWorldActionBase* Ptr)
	{
		Ptr->RunAction();
	});
}

void UWorldActionManager::RunConstruction()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	ForEachAction([this](FWorldActionBase* Ptr)
	{
		Ptr->OnConstruction();
		if (Ptr->ShouldTick())
		{
			PrimaryComponentTick.bStartWithTickEnabled = true;
		}
	});
}

void UWorldActionManager::ForEachAction(const TFunction<void(FWorldActionBase*)>& Func)
{
	for (auto It = ActionPtrs.CreateIterator(); It; ++It)
	{
		if (FWorldActionBase* Ptr = *It)
		{
			Ptr->SetWorldContext(this);
			if (Func) Func(Ptr);
		}
		else
		{
			It.RemoveCurrentSwap();
		}
	}
}

void UWorldActionManager::BeginPlay()
{
	Super::BeginPlay();
	ForEachAction([](FWorldActionBase* Ptr)
	{
		Ptr->OnBeginPlay();
	});
}

void UWorldActionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	ForEachAction([DeltaTime](FWorldActionBase* Ptr)
	{
		Ptr->OnTick(DeltaTime);
	});
}

#if WITH_EDITOR
void UWorldActionManager::PostLoad()
{
	Super::PostLoad();
	if (bAutoConstruction) RunConstruction();
}

void UWorldActionManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (bAutoConstruction) RunConstruction();
}
#endif