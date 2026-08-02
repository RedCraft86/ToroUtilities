// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroMathLibrary.h"

double UToroMathLibrary::GetHorizontalDistance(const FVector A, const FVector B)
{
	return FVector::Dist2D(A, B);
}

FRotator UToroMathLibrary::Add_RotatorRotator(const FRotator A, const FRotator B)
{
	return FRotator(A.Quaternion() * B.Quaternion());
}

FLinearColor UToroMathLibrary::TemperatureToLinearColor(const float Temperature)
{
	return FLinearColor::MakeFromColorTemperature(Temperature);
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

FColor UToroMathLibrary::TemperatureToColor(const float Temperature)
{
	return FColor::MakeFromColorTemperature(Temperature);
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

	Result.A = bRandomAlpha ? FMath::RandRange(0, 255) : 255;
	return Result;
}

float UToroMathLibrary::PerlinNoise2D(FVector2D Position)
{
	return FMath::PerlinNoise2D(Position);
}

float UToroMathLibrary::PerlinNoise3D(const FVector& Position)
{
	return FMath::PerlinNoise3D(Position);
}
