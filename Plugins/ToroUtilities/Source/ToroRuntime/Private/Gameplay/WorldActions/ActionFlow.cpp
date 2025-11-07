// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/ActionFlow.h"

FTimerManager* FWAFlow_Delay::GetTimerManager() const
{
	const UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(
		GetWorldContext(), EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	return World ? &World->GetTimerManager() : nullptr;
}

void FWAFlow_Delay::RunAction()
{
	FTimerManager* Manager = GetTimerManager();
	if (Manager && (bRetriggerable || Manager->IsTimerActive(DelayHandle)))
	{
		Manager->ClearTimer(DelayHandle);
		Manager->SetTimer(DelayHandle, [this]()
		{
			Actions.RunAction(GetWorldContext());
		}, Delay, false);
	}
}

void FWAFlow_Delay::OnBeginPlay()
{
	Actions.OnBeginPlay(GetWorldContext());
}

void FWAFlow_Delay::OnConstruction()
{
	Actions.OnConstruction(GetWorldContext());
	bShouldTick = Actions.ShouldTick();
}

void FWAFlow_Delay::OnTick(const float DeltaTime)
{
	Actions.OnTick(GetWorldContext(), DeltaTime);
}

void FWAFlow_DoOnce::RunAction()
{
	if (!bTriggered)
	{
		bTriggered = true;
		Actions.RunAction(GetWorldContext());
	}
}

void FWAFlow_DoOnce::OnBeginPlay()
{
	Actions.OnBeginPlay(GetWorldContext());
}

void FWAFlow_DoOnce::OnConstruction()
{
	Actions.OnConstruction(GetWorldContext());
	bShouldTick = Actions.ShouldTick();
}

void FWAFlow_DoOnce::OnTick(const float DeltaTime)
{
	Actions.OnTick(GetWorldContext(), DeltaTime);
}

void FWAFlow_FlipFlop::RunAction()
{
	bIsA = !bIsA;
	if (bIsA)
	{
		ActionsA.RunAction(GetWorldContext());
	}
	else
	{
		ActionsB.RunAction(GetWorldContext());
	}
}

void FWAFlow_FlipFlop::OnBeginPlay()
{
	ActionsA.OnBeginPlay(GetWorldContext());
	ActionsB.OnBeginPlay(GetWorldContext());
}

void FWAFlow_FlipFlop::OnConstruction()
{
	ActionsA.OnConstruction(GetWorldContext());
	ActionsB.OnConstruction(GetWorldContext());
	bShouldTick = ActionsA.ShouldTick() || ActionsB.ShouldTick();
}

void FWAFlow_FlipFlop::OnTick(const float DeltaTime)
{
	ActionsA.OnTick(GetWorldContext(), DeltaTime);
	ActionsB.OnTick(GetWorldContext(), DeltaTime);
}
