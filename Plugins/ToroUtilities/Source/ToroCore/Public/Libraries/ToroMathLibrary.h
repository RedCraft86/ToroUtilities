// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DataTypes/MathTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroMathLibrary.generated.h"

UCLASS(DisplayName = "Toro Math Library")
class TOROCORE_API UToroMathLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Returns UE_SMALL_NUMBER (1.e-8) */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Small"))
		static float SmallNumber();

	/* Returns UE_KINDA_SMALL_NUMBER (1.e-4) */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Kinda Small"))
		static float KindaSmallNumber();

	/* Returns UE_BIG_NUMBER (3.4e+38) */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Big"))
		static float BigNumber();

	/* Checks if a Location is in front of the Target. */
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (DefaultToSelf = "Target"))
		static bool IsLocationInFront(const AActor* Target, const FVector& Location);

	/* Checks if an Actor is in front of the Target. */
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (DefaultToSelf = "Target"))
		static bool IsActorInFront(const AActor* Target, const AActor* ActorToTest);

	/* Gets the XY distance between 2 vectors, ignoring height difference */
	UFUNCTION(BlueprintPure, Category = "Math|Vector")
		static float GetHorizontalDistance(const FVector& A, const FVector& B);

	/** Calculates the line trace vectors from a base location.
	* @param Location - Base Location to start the trace from
	* @param Rotation - Rotation to identify the various vectors from.
	* @param Direction - Direction of the Line Trace
	* @param Distance - Distance of the Line Trace
	* @param Start - Starting Line Trace Point
	* @param End - Ending Line Trace Point
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Vector")
		static void GetLineTraceVectors(const FVector& Location, const FRotator& Rotation,
			const EVectorDirection Direction, const float Distance, FVector& Start, FVector& End);

	/** Calculates the line trace vectors from an actor.
	* @param Actor - Actor to get base location from
	* @param Direction - Direction of the Line Trace
	* @param Distance - Distance of the Line Trace
	* @param Start - Starting Line Trace Point
	* @param End - Ending Line Trace Point
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (DefaultToSelf = "Actor"))
		static void GetActorLineTraceVectors(const AActor* Actor, const EVectorDirection Direction,
			const float Distance, FVector& Start, FVector& End);

	/** Calculates the line trace vectors from a component.
	* @param Component - Component to get base location from
	* @param Direction - Direction of the Line Trace
	* @param Distance - Distance of the Line Trace
	* @param Start - Starting Line Trace Point
	* @param End - Ending Line Trace Point
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (DefaultToSelf = "Component"))
		static void GetComponentLineTraceVectors(const USceneComponent* Component, const EVectorDirection Direction,
			const float Distance, FVector& Start, FVector& End);

	/** Calculates the line trace vectors from the player camera.
	* @param Direction - Direction of the Line Trace
	* @param Distance - Distance of the Line Trace
	* @param Start - Starting Line Trace Point
	* @param End - Ending Line Trace Point
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (WorldContext = "ContextObject", AdvancedDisplay = "PlayerIndex"))
		static void GetCameraLineTraceVectors(const UObject* ContextObject, const EVectorDirection Direction,
			const float Distance, FVector& Start, FVector& End, const int32 PlayerIndex = 0);
	
	/* Rotator Addition */
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (DisplayName = "rotator + rotator", CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"))
		static FRotator Add_RotatorRotator(const FRotator& A, const FRotator& B);
	
	/* Creates a Linear Color from the given Hex code */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor LinearColorFromHex(const FString& InHex);
	
	/* Creates a Hex code from the given Linear Color */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FString LinearColorToHex(const FLinearColor InColor, const bool IsSRGB);

	/* Creates a Linear Color from Temperature */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor LinearColorFromTemperature(const float InTemperature);
	
	/* Creates a random Linear Color */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor RandomLinearColor(const bool bTrueRandom, const bool bRandomAlpha);

	/* Creates a Color from the given Hex code */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor ColorFromHex(const FString InHex);
	
	/* Creates a Hex code from the given Color */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FString ColorToHex(const FColor InColor);
	
	/* Creates a Color from Temperature */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor ColorFromTemperature(const float InTemperature);
	
	/* Creates a random Color */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor RandomColor(const bool bTrueRandom, const bool bRandomAlpha);

	/** Get Date Time from Ticks
	* @param InTicks - Number of ticks since midnight, January 1, 0001
	*/
	UFUNCTION(BlueprintPure, Category = "Math|DateTime", meta = (DisplayName = "From Ticks (Date Time)"))
		static FDateTime GetDateTimeFromTicks(const int64 InTicks);
	
	/** Get Ticks from DateTime
	* @return Number of ticks since midnight, January 1, 0001
	*/
	UFUNCTION(BlueprintPure, Category = "Math|DateTime", meta = (DisplayName = "Get Ticks (Date Time)"))
		static int64 GetDateTimeTicks(const FDateTime& InDateTime);

	/** Get Timespan from Ticks
	* @param InTicks - Number of ticks since midnight, January 1, 0001
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Timespan", meta = (DisplayName = "From Ticks (Timespan)"))
		static FTimespan GetTimespanFromTicks(const int64 InTicks);

	/** Gets Ticks from FTimespan 
	* @return Number of ticks represented by this time span
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Timespan", meta = (DisplayName = "Get Ticks (Timespan)"))
		static int64 GetTimespanTicks(const FTimespan& InTimespan);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Inline Float Curve)")
		static bool HasInlineCurveData_Float(const FInlineFloatCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Inline Float Curve)")
		static UCurveFloat* GetInlineCurveAsset_Float(const FInlineFloatCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Inline Float Curve)")
		static float GetInlineCurveValue_Float(const FInlineFloatCurve& Target, float InTime);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Inline Float Curve)")
		static void GetInlineCurveTimeRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Inline Float Curve)")
		static void GetInlineCurveValueRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max);
	
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Inline Vector Curve)")
		static bool HasInlineCurveData_Vector(const FInlineVectorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Inline Vector Curve)")
		static UCurveVector* GetInlineCurveAsset_Vector(const FInlineVectorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Inline Vector Curve)")
		static FVector GetInlineCurveValue_Vector(const FInlineVectorCurve& Target, float InTime);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Inline Vector Curve)")
		static void GetInlineCurveTimeRange_Vector(const FInlineVectorCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Inline Vector Curve)")
		static void GetInlineCurveValueRange_Vector(const FInlineVectorCurve& Target, FVector& Min, FVector& Max);
	
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Inline Color Curve)")
		static bool HasInlineCurveData_Color(const FInlineColorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Inline Color Curve)")
		static UCurveLinearColor* GetInlineCurveAsset_Color(const FInlineColorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Inline Color Curve)")
		static FLinearColor GetInlineCurveValue_Color(const FInlineColorCurve& Target, float InTime);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Inline Color Curve)")
		static void GetInlineCurveTimeRange_Color(const FInlineColorCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Inline Color Curve)")
		static void GetInlineCurveValueRange_Color(const FInlineColorCurve& Target, FLinearColor& Min, FLinearColor& Max);
};
