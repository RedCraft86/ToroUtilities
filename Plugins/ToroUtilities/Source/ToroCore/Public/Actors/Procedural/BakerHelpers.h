// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Engine/SplineMeshActor.h"
#include "DataTypes/PrimitiveData.h"
#include "Components/InstancedStaticMeshComponent.h"
#if WITH_EDITOR
#include "Subsystems/EditorActorSubsystem.h"

namespace BakerHelpers
{
	template <typename T>
	T* SpawnActor(UEditorActorSubsystem* Subsystem, const FTransform& Transform)
	{
		T* Actor = Cast<T>(Subsystem->SpawnActorFromClass(T::StaticClass(),
			Transform.GetLocation(), Transform.GetRotation().Rotator()));
		Actor->SetActorScale3D(Transform.GetScale3D());
		return Actor;
	}

	TOROCORE_API inline AStaticMeshActor* CreateStaticMesh(UEditorActorSubsystem* Subsystem, const UStaticMeshComponent* Source)
	{
		FStaticMeshData Data;
		Data.FromStaticMeshComponent(Source);
		if (AStaticMeshActor* Actor = Data.IsValidData() ? SpawnActor<AStaticMeshActor>(Subsystem, Source->GetComponentTransform()) : nullptr)
		{
			UStaticMeshComponent* Comp = Actor->GetStaticMeshComponent();
			Comp->SetAffectDynamicIndirectLighting(Source->bAffectDynamicIndirectLighting);
			Comp->SetOverlayMaterial(Source->GetOverlayMaterial());
			Data.ToStaticMeshComponent(Comp);

			const AActor* Owner = Source->GetOwner();
			Actor->SetActorHiddenInGame(Owner->IsHidden());
			Actor->SetFolderPath(*(Owner->GetFolderPath().ToString() / Owner->GetActorLabel() + TEXT("_BAKED")));

			return Actor;
		}
		return nullptr;
	}

	TOROCORE_API inline ASplineMeshActor* CreateSplineMesh(UEditorActorSubsystem* Subsystem, const USplineMeshComponent* Source)
	{
		FStaticMeshData Data;
		Data.FromStaticMeshComponent(Source);
		if (ASplineMeshActor* Actor = Data.IsValidData() ? SpawnActor<ASplineMeshActor>(Subsystem, Source->GetComponentTransform()) : nullptr)
		{
			const FVector StartPos	= Source->GetStartPosition();
			const FVector StartTan	= Source->GetStartTangent();
			const FVector EndPos	= Source->GetEndPosition();
			const FVector EndTan	= Source->GetEndTangent();

			USplineMeshComponent* Comp = Actor->GetSplineMeshComponent();
			Comp->SetForwardAxis(Source->GetForwardAxis(), false);
			Comp->SetStartAndEnd(StartPos, StartTan, EndPos, EndTan, true);
			Comp->SetAffectDynamicIndirectLighting(Source->bAffectDynamicIndirectLighting);
			Comp->SetOverlayMaterial(Source->GetOverlayMaterial());
			Data.ToStaticMeshComponent(Comp);

			const AActor* Owner = Source->GetOwner();
			Actor->SetActorHiddenInGame(Owner->IsHidden());
			Actor->SetFolderPath(*(Owner->GetFolderPath().ToString() / Owner->GetActorLabel() + TEXT("_BAKED")));

			return Actor;
		}
		return nullptr;
	}

	TOROCORE_API inline TArray<AActor*> BakeComponent(UEditorActorSubsystem* Subsystem, const UStaticMeshComponent* Component, const int32 Idx)
	{
		TArray<AActor*> Actors;
		if (const UInstancedStaticMeshComponent* ISM = Cast<UInstancedStaticMeshComponent>(Component))
		{
			Actors.Reserve(ISM->GetNumInstances());
			for (int i = 0; i < ISM->GetNumInstances(); i++)
			{
				FTransform T;
				if (ISM->GetInstanceTransform(i, T, true))
				{
					if (AActor* Actor = CreateStaticMesh(Subsystem, ISM))
					{
						Actor->SetActorTransform(T);
						Actor->SetActorLabel(FString::Printf(TEXT("%s_%d_Inst_%d"),
							*ISM->GetStaticMesh()->GetName(), Idx, i));

						Actors.Add(Actor);
					}
				}
			}
		}
		else if (const USplineMeshComponent* SMC = Cast<USplineMeshComponent>(Component))
		{
			if (AActor* Actor = CreateSplineMesh(Subsystem, SMC))
			{
				Actor->SetActorLabel(FString::Printf(TEXT("%s_%d"),
					*SMC->GetStaticMesh()->GetName(), Idx));

				Actors.Add(Actor);
			}
		}
		else if (AActor* Actor = CreateStaticMesh(Subsystem, Component))
		{
			Actor->SetActorLabel(FString::Printf(TEXT("%s_%d"),
				*Component->GetStaticMesh()->GetName(), Idx));

			Actors.Add(Actor);
		}

		return Actors;
	}
}
#endif