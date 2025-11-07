// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/WorldActionBase.h"

void FWorldActionArray::RunAction(const UObject* Context)
{
	for (TInstancedStruct<FWorldActionBase>& Action : Actions)
	{
		if (FWorldActionBase* Ptr = Action.GetMutablePtr())
		{
			Ptr->SetWorldContext(Context);
			Ptr->RunAction();
		}
	}
}

void FWorldActionArray::OnBeginPlay(const UObject* Context)
{
	for (TInstancedStruct<FWorldActionBase>& Action : Actions)
	{
		if (FWorldActionBase* Ptr = Action.GetMutablePtr())
		{
			Ptr->SetWorldContext(Context);
			Ptr->OnBeginPlay();
		}
	}
}

void FWorldActionArray::OnConstruction(const UObject* Context)
{
	bShouldTick = false;
	for (TInstancedStruct<FWorldActionBase>& Action : Actions)
	{
		if (FWorldActionBase* Ptr = Action.GetMutablePtr())
		{
			Ptr->SetWorldContext(Context);
			Ptr->OnConstruction();
			if (Ptr->ShouldTick())
			{
				bShouldTick = true;
			}
		}
	}
}

void FWorldActionArray::OnTick(const UObject* Context, const float DeltaTime)
{
	if (!bShouldTick) return;
	for (TInstancedStruct<FWorldActionBase>& Action : Actions)
	{
		FWorldActionBase* Ptr = Action.GetMutablePtr();
		if (Ptr && Ptr->ShouldTick())
		{
			Ptr->SetWorldContext(Context);
			Ptr->OnTick(DeltaTime);
		}
	}
}

TArray<FWorldActionBase*> FWorldActionArray::GetPtrs()
{
	TArray<FWorldActionBase*> Result;
	Result.Reserve(Actions.Num());
	for (TInstancedStruct<FWorldActionBase>& Action : Actions)
	{
		if (FWorldActionBase* Ptr = Action.GetMutablePtr())
		{
			Result.Add(Ptr);
		}
	}
	return Result;
}
