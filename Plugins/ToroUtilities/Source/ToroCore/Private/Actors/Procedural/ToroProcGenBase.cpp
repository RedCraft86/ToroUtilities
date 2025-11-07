// Copyright (C) RedCraft86. All Rights Reserved.

#include "Actors/Procedural/ToroProcGenBase.h"
#include "Algo/RandomShuffle.h"
#if WITH_EDITOR
#include "Actors/Procedural/BakerHelpers.h"
#endif

AToroProcGenBase::AToroProcGenBase(): bRealtimeConstruction(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
#if WITH_EDITORONLY_DATA
	SceneRoot->bVisualizeComponent = true;
	bRunConstructionScriptOnDrag = true;
#endif
}

void AToroProcGenBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bRealtimeConstruction) Construct();
}

#if WITH_EDITOR
void AToroProcGenBase::BakeInternal(const bool bRemoveSource, const bool bSelectActors, TArray<AActor*>* OutActors)
{
	const FScopedTransaction Transaction(NSLOCTEXT("ToroCore", "BakeProceduralActor", "Bake Procedural Actor"));

	UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr;
	if (!Subsystem) return;
	
	TArray<UStaticMeshComponent*> Comps;
	GetComponents<UStaticMeshComponent>(Comps);

	TArray<AActor*> Actors;
	for (int32 i = 0; i < Comps.Num(); i++)
	{
		Actors.Append(BakerHelpers::BakeComponent(Subsystem, Comps[i], i));
	}

	if (OutActors)
	{
		OutActors->Empty(Actors.Num());
		OutActors->Append(Actors);
	}
	if (bSelectActors)
	{
		Subsystem->SetSelectedLevelActors(Actors);
	}
	if (bRemoveSource)
	{
		Subsystem->SetActorSelectionState(this, false);
		Subsystem->DestroyActor(this);
	}
}
#endif

TArray<int32> AToroProcGenBase::LoopMeshArray(const TArray<FTransformMeshData>& Sample,
	const EGeneratorLoopMode Mode, const uint8 Amount)
{
	if (Sample.IsEmpty() || Amount == 0) 
		return {};

	TArray<int32> FilteredIndices;
	FilteredIndices.Reserve(Sample.Num()); 
	for (int32 i = 0; i < Sample.Num(); ++i)
	{
		if (Sample[i].IsValidData())
		{
			FilteredIndices.Add(i);
		}
	}

	if (FilteredIndices.IsEmpty()) 
		return FilteredIndices;

	TArray<int32> RetVal;
	RetVal.Reserve(Amount);
	for (int32 i = 0; i < Amount; ++i)
	{
		RetVal.Add(FilteredIndices[i % FilteredIndices.Num()]);
	}

	switch (Mode)
	{
	case EGeneratorLoopMode::Reverse:
		Algo::Reverse(RetVal);
		break;

	case EGeneratorLoopMode::Random:
		Algo::RandomShuffle(RetVal);
		break;

	default:
		break;
	}

	return RetVal;
}
