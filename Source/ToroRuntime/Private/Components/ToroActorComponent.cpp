// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Components/ToroActorComponent.h"
#include "ToroRuntime.h"

UToroActorComponent::UToroActorComponent(): MaxInstancesPerActor(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UToroActorComponent::OnRegister()
{
	Super::OnRegister();
	const AActor* OwnerActor = GetOwner();
	if (MaxInstancesPerActor > 0 && OwnerActor)
	{
		TArray<UToroActorComponent*> Components;
		OwnerActor->GetComponents(GetClass(), Components);
		const int32 ComponentIndex = Components.IndexOfByKey(this);
		if (ComponentIndex >= MaxInstancesPerActor)
		{
			UE_LOG(LogToroRuntime, Warning,
				TEXT("Actor %s[%s] exceeds the maximum instance limit (%d) for component %s. Deactivating %s."),
				*OwnerActor->GetClass()->GetName(), *OwnerActor->GetPathName(), MaxInstancesPerActor, *GetClass()->GetName(), *GetName()
			);

			Deactivate();
			bAutoActivate = false;
		}
	}
}
