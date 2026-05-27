// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/CollisionProfile.h"
#include "PrimitiveCollision.generated.h"
	 
/**
 * A comprehensive wrapper for Unreal Engine collision settings.
 *
 * This struct encapsulates all data required to define a primitive's collision behavior, 
 * including its profile name, enabled state, object type, and specific channel responses.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FPrimitiveCollision final
{
	GENERATED_BODY()

private:
// #if WITH_EDITOR
// 	friend class FPrimitiveCollisionDetails; // TODO details visualization
// #endif
	 
	/** 
	 * Determines if the component is enabled for queries (traces/overlaps) and/or physics simulation. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;

	/** 
	 * The logical 'type' of this object (e.g., WorldStatic, Pawn, Projectile). 
	 * Used as the filter key when other objects check their responses to this object.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true)) 
		TEnumAsByte<ECollisionChannel> ObjectType;

	/** 
	 * Map of collision channels to their specific responses (Ignore, Overlap, or Block). 
	 * Only used when ProfileName is set to "Custom".
	 */
	UPROPERTY(BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> ResponsesMap;

	/** 
	 * The name of the collision profile (preset) to use. 
	 * If set to "Custom", the individual settings in this struct are used instead of a global preset.
	 */
	UPROPERTY(BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		FName ProfileName;

public:

	FPrimitiveCollision()
		: CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
		, ObjectType(ECC_WorldStatic)
		, ResponsesMap({})
	{
        SetProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	FPrimitiveCollision(const TEnumAsByte<ECollisionEnabled::Type> InEnabled,
			const TEnumAsByte<ECollisionChannel> InObjectType = ECC_WorldStatic,
			const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses = {})
		: CollisionEnabled(InEnabled)
		, ObjectType(InObjectType)
		, ResponsesMap(InResponses)
		, ProfileName(UCollisionProfile::CustomCollisionProfileName)
	{
		FillDefaultResponses();
	}

	FPrimitiveCollision(const FName& ProfileName)
		: CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
		, ObjectType(ECC_WorldStatic)
		, ResponsesMap({})
	{
		SetProfileName(ProfileName);
	}

	bool HasValidProfileName() const;
	bool SetProfileName(const FName& InName);
	FName GetProfileName() const { return ProfileName; }

	void SetCollisionEnabled(const ECollisionEnabled::Type InEnabled);
	ECollisionEnabled::Type GetCollisionEnabled() const { return CollisionEnabled; }

	void SetObjectType(const ECollisionChannel InObjectType);
	ECollisionChannel GetObjectType() const { return ObjectType; }

	void SetAllResponses(const ECollisionResponse InResponse);
	void SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse);
	ECollisionResponse GetResponse(const ECollisionChannel InChannel) const;

	void FillDefaultResponses(const ECollisionResponse InResponses = ECR_Ignore, const bool bResetAll = false);
	void SetResponses(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Map);
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& GetResponsesMap() const { return ResponsesMap; }

	void FromPrimitiveComponent(const UPrimitiveComponent* Target);
	void ToPrimitiveComponent(UPrimitiveComponent* Target) const;

	struct Utils
	{
		/** 
		 * Retrieves raw collision data for a named profile from the engine. 
		 * @param ProfileName The name of the profile to search for.
		 * @param OutData (Optional) Pointer to receive the template data if found.
		 * @return True if the profile exists.
		 */
		static bool GetProfile(const FName& ProfileName, FCollisionResponseTemplate* OutData = nullptr);

		/** @return True if the channel is valid and not 'None'. */
		static bool IsValidChannel(const ECollisionChannel InChannel);

		/** @return True if the channel is marked as hidden in project settings. (Editor-Only) */
		static bool IsHiddenChannel(const ECollisionChannel InChannel);

		/** 
		 * Iterates through all available collision channels.
		 * @param Func The callback function to execute for each channel.
		 * @param bIgnoreHidden If true, skip channels marked as hidden.
		 */
		static void ForEachChannel(const TFunction<void(const ECollisionChannel, const int32, const UEnum*)>& Func, const bool bIgnoreHidden);
	};
};