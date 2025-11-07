// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroMathLibrary.h"
#include "Kismet/GameplayStatics.h"

float UToroMathLibrary::SmallNumber()
{
	return UE_SMALL_NUMBER;
}

float UToroMathLibrary::KindaSmallNumber()
{
	return UE_KINDA_SMALL_NUMBER;
}

float UToroMathLibrary::BigNumber()
{
	return UE_BIG_NUMBER;
}

bool UToroMathLibrary::IsLocationInFront(const AActor* Target, const FVector& Location)
{
	if (IsValid(Target))
	{
		FVector DotVec = Location - Target->GetActorLocation(); DotVec.Normalize();
		return FVector::DotProduct(Target->GetActorForwardVector(), DotVec) > 0.0f;
	}
	return false;
}

bool UToroMathLibrary::IsActorInFront(const AActor* Target, const AActor* ActorToTest)
{
	if (IsValid(Target) && IsValid(ActorToTest))
	{
		return IsLocationInFront(Target, ActorToTest->GetActorLocation());
	}
	return false;
}

float UToroMathLibrary::GetHorizontalDistance(const FVector& A, const FVector& B)
{
	return FVector::Dist2D(A, B);
}

void UToroMathLibrary::GetLineTraceVectors(const FVector& Location, const FRotator& Rotation,
	const EVectorDirection Direction, const float Distance, FVector& Start, FVector& End)
{
	FVector DirectionVector;
	switch (Direction)
	{
	case EVectorDirection::Forward:
		DirectionVector = Rotation.Vector();
		break;

	case EVectorDirection::Right:
		DirectionVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		break;

	case EVectorDirection::Up:
		DirectionVector = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Z);
		break;
	};

	Start = Location;
	End = Location + DirectionVector * Distance;
}

void UToroMathLibrary::GetActorLineTraceVectors(const AActor* Actor,
	const EVectorDirection Direction, const float Distance, FVector& Start, FVector& End)
{
	if (IsValid(Actor))
	{
		FVector DirectionVector;
		switch (Direction)
		{
		case EVectorDirection::Forward:
			DirectionVector = Actor->GetActorForwardVector();
			break;

		case EVectorDirection::Right:
			DirectionVector = Actor->GetActorRightVector();
			break;

		case EVectorDirection::Up:
			DirectionVector = Actor->GetActorUpVector();
			break;
		};

		Start = Actor->GetActorLocation();
		End = Actor->GetActorLocation() + DirectionVector * Distance;
	}
}

void UToroMathLibrary::GetComponentLineTraceVectors(const USceneComponent* Component,
	const EVectorDirection Direction, const float Distance, FVector& Start, FVector& End)
{
	if (IsValid(Component))
	{
		FVector DirectionVector;
		switch (Direction)
		{
		case EVectorDirection::Forward:
			DirectionVector = Component->GetForwardVector();
			break;

		case EVectorDirection::Right:
			DirectionVector = Component->GetRightVector();
			break;

		case EVectorDirection::Up:
			DirectionVector = Component->GetUpVector();
			break;
		};

		Start = Component->GetComponentLocation();
		End = Component->GetComponentLocation() + DirectionVector * Distance;
	}
}

void UToroMathLibrary::GetCameraLineTraceVectors(const UObject* ContextObject, const EVectorDirection Direction,
	const float Distance, FVector& Start, FVector& End, const int32 PlayerIndex)
{
	if (const APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(ContextObject, PlayerIndex))
	{
		FVector DirectionVector;
		switch (Direction)
		{
		case EVectorDirection::Forward:
			DirectionVector = PCM->GetActorForwardVector();
			break;

		case EVectorDirection::Right:
			DirectionVector = PCM->GetActorRightVector();
			break;

		case EVectorDirection::Up:
			DirectionVector = PCM->GetActorUpVector();
			break;
		};

		Start = PCM->GetCameraLocation();
		End = PCM->GetCameraLocation() + DirectionVector * Distance;
	}
}

FRotator UToroMathLibrary::Add_RotatorRotator(const FRotator& A, const FRotator& B)
{
	return FRotator(FQuat(A) * FQuat(B));
}

FLinearColor UToroMathLibrary::LinearColorFromHex(const FString& InHex)
{
	return FColor::FromHex(InHex);
}

FString UToroMathLibrary::LinearColorToHex(const FLinearColor InColor, const bool IsSRGB)
{
	return InColor.ToFColor(IsSRGB).ToHex();
}

FLinearColor UToroMathLibrary::LinearColorFromTemperature(const float InTemperature)
{
	return FLinearColor::MakeFromColorTemperature(InTemperature);
}

FLinearColor UToroMathLibrary::RandomLinearColor(const bool bTrueRandom, const bool bRandomAlpha)
{
	FLinearColor Result;
	if (bTrueRandom)
	{
		Result.R = FMath::FRand();
		Result.G = FMath::FRand();
		Result.B = FMath::FRand();
	}
	else
	{
		Result = FLinearColor::MakeRandomColor();
	}

	Result.A = bRandomAlpha ? FMath::FRand() : 1.0f;
	return Result;
}

FColor UToroMathLibrary::ColorFromHex(const FString InHex)
{
	return FColor::FromHex(InHex);
}

FString UToroMathLibrary::ColorToHex(const FColor InColor)
{
	return InColor.ToHex();
}

FColor UToroMathLibrary::ColorFromTemperature(const float InTemperature)
{
	return FColor::MakeFromColorTemperature(InTemperature);
}

FColor UToroMathLibrary::RandomColor(const bool bTrueRandom, const bool bRandomAlpha)
{
	FColor Result;
	if (bTrueRandom)
	{
		Result.R = FMath::RandRange(0, 255);
		Result.G = FMath::RandRange(0, 255);
		Result.B = FMath::RandRange(0, 255);
	}
	else
	{
		Result = FColor::MakeRandomColor();
	}

	Result.A = bRandomAlpha ? FMath::FRand() : 255;
	return Result;
}

FDateTime UToroMathLibrary::GetDateTimeFromTicks(const int64 InTicks)
{
	return FDateTime(InTicks);
}

int64 UToroMathLibrary::GetDateTimeTicks(const FDateTime& InDateTime)
{
	return InDateTime.GetTicks();
}

FTimespan UToroMathLibrary::GetTimespanFromTicks(const int64 InTicks)
{
	return FTimespan(InTicks);
}

int64 UToroMathLibrary::GetTimespanTicks(const FTimespan& InTimespan)
{
	return InTimespan.GetTicks();
}

bool UToroMathLibrary::HasInlineCurveData_Float(const FInlineFloatCurve& Target)
{
	return Target.HasAnyData();
}

UCurveFloat* UToroMathLibrary::GetInlineCurveAsset_Float(const FInlineFloatCurve& Target)
{
	return Target.GetCurveAsset();
}

float UToroMathLibrary::GetInlineCurveValue_Float(const FInlineFloatCurve& Target, float InTime)
{
	return Target.GetValue(InTime);
}

void UToroMathLibrary::GetInlineCurveTimeRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max)
{
	Target.GetTimeRange(Min, Max);
}

void UToroMathLibrary::GetInlineCurveValueRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max)
{
	Target.GetValueRange(Min, Max);
}

bool UToroMathLibrary::HasInlineCurveData_Vector(const FInlineVectorCurve& Target)
{
	return Target.HasAnyData();
}

UCurveVector* UToroMathLibrary::GetInlineCurveAsset_Vector(const FInlineVectorCurve& Target)
{
	return Target.GetCurveAsset();
}

FVector UToroMathLibrary::GetInlineCurveValue_Vector(const FInlineVectorCurve& Target, float InTime)
{
	return Target.GetValue(InTime);
}

void UToroMathLibrary::GetInlineCurveTimeRange_Vector(const FInlineVectorCurve& Target, float& Min, float& Max)
{
	Target.GetTimeRange(Min, Max);
}

void UToroMathLibrary::GetInlineCurveValueRange_Vector(const FInlineVectorCurve& Target, FVector& Min, FVector& Max)
{
	Target.GetValueRange(Min, Max);
}

bool UToroMathLibrary::HasInlineCurveData_Color(const FInlineColorCurve& Target)
{
	return Target.HasAnyData();
}

UCurveLinearColor* UToroMathLibrary::GetInlineCurveAsset_Color(const FInlineColorCurve& Target)
{
	return Target.GetCurveAsset();
}

FLinearColor UToroMathLibrary::GetInlineCurveValue_Color(const FInlineColorCurve& Target, float InTime)
{
	return Target.GetValue(InTime);
}

void UToroMathLibrary::GetInlineCurveTimeRange_Color(const FInlineColorCurve& Target, float& Min, float& Max)
{
	Target.GetTimeRange(Min, Max);
}

void UToroMathLibrary::GetInlineCurveValueRange_Color(const FInlineColorCurve& Target, FLinearColor& Min, FLinearColor& Max)
{
	Target.GetValueRange(Min, Max);
}
