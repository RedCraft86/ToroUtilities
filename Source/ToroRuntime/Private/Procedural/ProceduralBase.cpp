// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Procedural/ProceduralBase.h"
#include "Algo/RandomShuffle.h"

AProceduralBase::AProceduralBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

#if WITH_EDITORONLY_DATA
	SceneRoot->bVisualizeComponent = true;
	bRunConstructionScriptOnDrag = true;
#endif

	bRealtimeConstruction = true;

	SetCanBeDamaged(false);
}

TArray<uint8> AProceduralBase::LoopMeshArray(const TArray<FStaticMeshProperties>& Sample, const EProceduralLoopMode Mode, const uint8 Amount)
{
	
	if (Sample.IsEmpty() || Amount == 0)
	{
		return {};
	}

	TArray<uint8> IdxSample;
	for (uint8 i = 0; i < Sample.Num(); i++)
	{
		IdxSample.Add(i);
		if (i == UINT_MAX)
		{
			break;
		}
	}

	if (Mode == EProceduralLoopMode::Reverse)
	{
		Algo::Reverse(IdxSample);
	}
	else if (Mode == EProceduralLoopMode::Random)
	{
		Algo::RandomShuffle(IdxSample);
	}

	TArray<uint8> Result;
	while (Result.Num() < Amount)
	{
		Result.Append(IdxSample);
	}

	Result.SetNum(Amount, EAllowShrinking::Yes);

	if (Mode == EProceduralLoopMode::Random)
	{
		Algo::RandomShuffle(Result);
	}

	return Result;
}

void AProceduralBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bRealtimeConstruction)
	{
		Construct();
	}
}
