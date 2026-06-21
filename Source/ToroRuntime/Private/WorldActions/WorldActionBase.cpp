// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "WorldActions/WorldActionBase.h"
#include "Helpers/WorldGetter.h"
#include "ToroRuntime.h"
#if WITH_EDITOR
#include "Misc/App.h"
#endif

void UWorldActionBase::SetWorldContext(UObject* InContext)
{
	if (InContext && InContext != this)
	{
		WorldContext = InContext;
	}
}

bool UWorldActionBase::RunAction()
{
	if (bRunOnce && bHasRan)
	{
		return false;
	}

	if (!GetWorld())
	{
		UE_LOG(LogToroRuntime, Error, 
			TEXT("Tried to run world action %s[%s] but it does not appear to have world context."), 
			*GetClass()->GetName(), *GetName());
		return false;
	}

	bHasRan = true;
	if (Delay < 0.1f)
	{
		OnExecute();
		DelayTimer.Invalidate();
		return true;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (bRetriggerable || !TimerManager.TimerExists(DelayTimer))
	{
		TimerManager.ClearTimer(DelayTimer);
		TimerManager.SetTimer(DelayTimer, this, &UWorldActionBase::OnExecute, Delay, false);
		return true;
	}

	return false;
}

UWorld* UWorldActionBase::GetWorld() const
{
#if WITH_EDITOR
	return FApp::IsGame() ? FWorldGetter::Get(WorldContext.Get()) : GEngine->GetCurrentPlayWorld();
#else
	return FWorldGetter::Get(WorldContext.Get());
#endif
}

void UTickableWorldActionBase::BeginDestroy()
{
	SetTickableTickType(ETickableTickType::Never);
	Super::BeginDestroy();
}

void UTickableWorldActionBase::PostInitProperties()
{
	Super::PostInitProperties();
	if (!IsTemplate())
	{
		SetTickableTickType(ETickableTickType::Conditional);
	}
}
