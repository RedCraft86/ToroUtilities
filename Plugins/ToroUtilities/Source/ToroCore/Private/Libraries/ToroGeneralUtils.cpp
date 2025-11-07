// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroGeneralUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

TArray<FVector> FActorBoundsCheckParams::ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const
{
	TArray<FVector> Out{Origin};
	for (const FVector& Vector : InVertices)
	{
		Out.AddUnique(FVector(
			FMath::Lerp(Origin.X, Vector.X, BoundingBoxLerp.X),
			FMath::Lerp(Origin.Y, Vector.Y, BoundingBoxLerp.Y),
			FMath::Lerp(Origin.Z, Vector.Z, BoundingBoxLerp.Z)
		));
	}
	return Out;
}

UWorld* UToroGeneralUtils::GetPlayWorld(const UObject* Context)
{
	UWorld* World = GEngine->GetWorldFromContextObject(Context, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) World = GEngine->GetCurrentPlayWorld();
	return World ? World : GWorld;
}

TArray<FVector> UToroGeneralUtils::GetBoundingBoxVertices(const AActor* Target, const bool bOnlyCollidingComponents,
	const bool bIncludeFromChildActors, FVector& Origin, FVector& BoxExtent)
{
	TArray<FVector> Result;
	if (IsValid(Target))
	{
		Target->GetActorBounds(bOnlyCollidingComponents, Origin, BoxExtent, bIncludeFromChildActors);
	
		Result.AddUnique(Origin + BoxExtent);
		Result.AddUnique(Origin - BoxExtent);
		Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z + BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z + BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z - BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z + BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z - BoxExtent.Z));
		Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z - BoxExtent.Z));
	}
	return Result;
}

bool UToroGeneralUtils::IsActorOnScreen(const AActor* Target, const float MaxDistance,
	const bool bOriginOnly, const bool bLineTrace, const FActorBoundsCheckParams& TraceParams)
{
	if (!IsValid(Target) || MaxDistance <= 0 || (bLineTrace && TraceParams.BoundingBoxLerp.GetMin() <= 0))
	{
		return false;
	}

	APlayerController* Controller = UGameplayStatics::GetPlayerController(Target, 0);
	if (!Controller || !Controller->GetLocalPlayer()) return false;
	
	ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
	FSceneViewFamilyContext ViewFamily(
		FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, Target->GetWorld()->Scene,
			LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true)
	);
	
	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);
	if (FVector::Distance(ViewLocation, Target->GetActorLocation()) > MaxDistance)
	{
		return false;
	}

	FVector Origin, BoxExtent = FVector::ZeroVector;
	Target->GetActorBounds(false, Origin, BoxExtent, false);
	if (!SceneView || !SceneView->ViewFrustum.IntersectBox(Origin, BoxExtent))
	{
		return false;
	}
	
	TArray<FVector> TestVectors;
	TestVectors.Add(Target->GetActorLocation());

	if (!bOriginOnly)
	{
		TArray<FVector> BoundingBoxVertices = GetBoundingBoxVertices(
			Target, TraceParams.bOnlyCollidingComponents,
			TraceParams.bIncludeFromChildActors, Origin, BoxExtent
		);
		
		TestVectors = TraceParams.ProcessVertices(BoundingBoxVertices, Origin);
	}

	bool bPassedScreenTest = false;
	for (int i = 0; i < TestVectors.Num(); i++)
	{
		FVector2D ScreenLoc;
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(Controller);
		
		if (!Controller->ProjectWorldLocationToScreen(TestVectors[i], ScreenLoc)) continue;
		if (ScreenLoc.X >= 0 && ScreenLoc.X <= ViewportSize.X && ScreenLoc.Y >= 0 && ScreenLoc.Y <= ViewportSize.Y)
		{
			bPassedScreenTest = true;
			break;
		}
	}

	if (!bPassedScreenTest) return false;
	if (!bLineTrace) return true;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Target);
	QueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(Target, 0));

	FHitResult HitResult;
	for (int i = 0; i < TestVectors.Num(); i++)
	{
		if (!Target->GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation,
			TestVectors[i], TraceParams.LineTraceChannel, QueryParams))
			return true;
	}

	return false;
}

void UToroGeneralUtils::ForceGarbageCollection()
{
	if (GEngine)
	{
#if WITH_EDITOR
		GEngine->ForceGarbageCollection(false);
#else
		GEngine->ForceGarbageCollection(true);
#endif
	}
}

int32 UToroGeneralUtils::GetNumAsyncPkgs()
{
	return GetNumAsyncPackages();
}

float UToroGeneralUtils::GetAsyncPkgLoadPercent(const FString Package)
{
	return GetAsyncLoadPercentage(*Package);
}

void UToroGeneralUtils::CallLocalEvent(UObject* Target, const FName EventName)
{
	if (IsValid(Target) && !EventName.IsNone())
	{
		FOutputDeviceNull Ar;
		Target->CallFunctionByNameWithArguments(*EventName.ToString(),
			Ar, nullptr, true);
	}
}

bool UToroGeneralUtils::IsInEditor()
{
#if WITH_EDITOR
	return !FApp::IsGame();
#else
	return false;
#endif
}

UActorComponent* UToroGeneralUtils::AddActorInstanceComponent(AActor* Target, const TSubclassOf<UActorComponent> InClass)
{
#if WITH_EDITOR
	if (Target && InClass) 
	{
		UActorComponent* NewComponent = NewObject<UActorComponent>(Target, InClass);
		NewComponent->RegisterComponent();
		Target->AddInstanceComponent(NewComponent);
		return NewComponent;
	}
#endif
	return nullptr;
}
