// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "WorldMusic/WorldMusicManager.h"
#include "TimerManager.h"

UWorldMusicManager::UWorldMusicManager()
	: TransitionTime(1.0f), bDipNullChecks(false), DippedVolume(0.5f), DippedLowPassFilter(800.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.TickInterval = 1.0f;

	bIsMusic = true;
	bAutoActivate = false;
	bCanPlayMultipleInstances = false;
	SetUISound(true);
}

void UWorldMusicManager::ChangeMusic(UMetaSoundSource* NewSource)
{
	if (!NewSource)
	{
		return;
	}

	if (!Sound)
	{
		SetSound(NewSource);
		ResetSoundParams();
		FadeIn(TransitionTime);
	}
	else if (Sound != NewSource)
	{
		FadeOut(TransitionTime * 0.5f, 0.0f);
		GetWorld()->GetTimerManager().ClearTimer(TransitionTimer);
		GetWorld()->GetTimerManager().SetTimer(TransitionTimer, [WeakThis = TWeakObjectPtr(this), NewSource]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->SetSound(NewSource);
				WeakThis->ResetSoundParams();
				WeakThis->FadeIn(WeakThis->TransitionTime * 0.5f);
			}
		}, TransitionTime * 0.5f, false);
	}
}

void UWorldMusicManager::AddDipRequest(const UObject* InRequester)
{
	DipRequests.AddRequest(InRequester);
}

void UWorldMusicManager::RemoveDipRequest(const UObject* InRequester)
{
	DipRequests.RemoveRequest(InRequester);
}

void UWorldMusicManager::ClearDipRequests()
{
	DipRequests.ClearRequests();
}

void UWorldMusicManager::ResetSoundParams()
{
	ResetParameters();

	for (const TPair<FName, bool>& Pair : DefaultBoolParams)
	{
		SetBoolParameter(Pair.Key, Pair.Value);
	}

	for (const TPair<FName, int32>& Pair : DefaultInt32Params)
	{
		SetIntParameter(Pair.Key, Pair.Value);
	}

	for (const TPair<FName, float>& Pair : DefaultFloatParams)
	{
		SetFloatParameter(Pair.Key, Pair.Value);
	}
}

void UWorldMusicManager::OnDipRequestChanged(const bool bState)
{
	SetLowPassFilterEnabled(bState);
	SetLowPassFilterFrequency(DippedLowPassFilter);
	AdjustVolume(TransitionTime * 0.5f, bState ? DippedVolume : 1.0f);
}

void UWorldMusicManager::BeginPlay()
{
	Super::BeginPlay();
	DipRequests.OnRequestChanged.AddUObject(this, &UWorldMusicManager::OnDipRequestChanged);
	SetComponentTickEnabled(bDipNullChecks);
	ChangeMusic(DefaultTheme);
}

void UWorldMusicManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	DipRequests.CleanupNulls();
}

#if WITH_EDITOR
void UWorldMusicManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (bRefreshParameters && DefaultTheme)
	{
		bRefreshParameters = false;
		const FMetasoundFrontendGraphClass& RootGraph = DefaultTheme->GetConstDocument().RootGraph;
		for (const FMetasoundFrontendClassInput& Input : RootGraph.Interface.Inputs)
		{
			static const TSet<FName> ValidTypes = { TEXT("Bool"), TEXT("Int32"), TEXT("Float") };
			if (!ValidTypes.Contains(Input.TypeName))
			{
				continue;
			}

			Input.IterateDefaults([this, Input](const FGuid&, const FMetasoundFrontendLiteral& Literal)
			{
				if (Literal.GetType() == EMetasoundFrontendLiteralType::Boolean)
				{
					bool bValue = false;
					Literal.TryGet(bValue);
					DefaultBoolParams.FindOrAdd(Input.Name, bValue);
				}
				else if (Literal.GetType() == EMetasoundFrontendLiteralType::Integer)
				{
					int32 Value = 0;
					Literal.TryGet(Value);
					DefaultInt32Params.FindOrAdd(Input.Name, Value);
				}
				else if (Literal.GetType() == EMetasoundFrontendLiteralType::Float)
				{
					float Value = 0.0f;
					Literal.TryGet(Value);
					DefaultFloatParams.FindOrAdd(Input.Name, Value);
				}
			});
		}
	}
}
#endif