// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PrimitiveCollision.generated.h"

/**
 * A standalone wrapper for the collision settings within FBodyInstance.
 * This struct encapsulates basic data required to define a primitive's collision behavior
 * such as profile name (optional), enabled state, object type, and specific channel responses.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FPrimitiveCollision final
{
	GENERATED_BODY()

private:
#if WITH_EDITOR
	// TODO: friend class FPrimitiveCollisionDetails
#endif

	/** 
	 * The name of the collision profile (preset) to use. 
	 * If set to "Custom", the individual settings in this struct are used instead of a global preset.
	 */
	UPROPERTY(BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		FName ProfileName;

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
		TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>> Responses;

public:

	FPrimitiveCollision()
		: CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
		, ObjectType(ECC_WorldStatic)
		, Responses({})
	{
		SetProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	FPrimitiveCollision(const TEnumAsByte<ECollisionEnabled::Type> InEnabled,
		const TEnumAsByte<ECollisionChannel> InObjectType = ECC_WorldStatic,
		const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses = {})
		: ProfileName(UCollisionProfile::CustomCollisionProfileName)
		, CollisionEnabled(InEnabled)
		, ObjectType(InObjectType)
		, Responses(InResponses)
	{
		FillAllResponses();
	}

	FPrimitiveCollision(const FName& ProfileName)
		: CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
		, ObjectType(ECC_WorldStatic)
		, Responses({})
	{
		SetProfileName(ProfileName);
	}

	bool HasValidProfileName() const;
	bool SetProfileName(const FName& InName);
	const FName& GetProfileName() const { return ProfileName; }

	void SetCollisionEnabled(const ECollisionEnabled::Type InEnabled);
	ECollisionEnabled::Type GetCollisionEnabled() const { return CollisionEnabled; }

	void SetObjectType(const ECollisionChannel InObjectType);
	ECollisionChannel GetObjectType() const { return ObjectType; }

	void SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse);
	ECollisionResponse GetResponse(const ECollisionChannel InChannel) const { return Responses.FindRef(InChannel); }

	void SetAllResponses(const ECollisionResponse InResponse);
	void FillAllResponses(const ECollisionResponse InResponse = ECR_Ignore, const bool bResetAll = false);

	void SetResponses(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses);
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& GetResponses() const { return Responses; }

	bool FromPrimitiveComponent(const UPrimitiveComponent* Target);
	bool ToPrimitiveComponent(UPrimitiveComponent* Target) const;

	bool Equals(const FPrimitiveCollision& Other) const;
	bool operator==(const FPrimitiveCollision& Other) const { return Equals(Other); }
	bool operator!=(const FPrimitiveCollision& Other) const { return !Equals(Other); }

	struct TOROCORE_API Utils final
	{
		/** 
		 * Retrieves collision info for a named profile. 
		 * @param ProfileName The name of the profile to search for.
		 * @param OutData (Optional) Pointer to receive the template data if found.
		 * @return True if the profile exists.
		 */
		static bool GetProfile(const FName& ProfileName, FCollisionResponseTemplate* OutData = nullptr);

		/** 
		 * Iterates through all available collision channels.
		 * @param Func The callback function to execute for each channel with params Channel, Index, Enum Object.
		 * @param bIgnoreHidden If true, skip channels marked as hidden.
		 */
		static void ForEachChannel(const TFunction<void(const ECollisionChannel, const int32, const UEnum*)>& Func, const bool bIgnoreHidden);
	};
};

/**
 * Blueprint function wrappers for PrimitiveCollision struct functions
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UPrimitiveCollisionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Updates the collision profile name. Assigning a valid name (e.g., "Pawn", "BlockAll") 
	 * will automatically update the object type and response map.
	 * @param Collision The primitive collision to modify.
	 * @param ProfileName The name of the project collision preset to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionProfileName(UPARAM(ref) FPrimitiveCollision& Collision, const FName ProfileName)
	{
		Collision.SetProfileName(ProfileName);
	}

	/** 
	 * @param Collision The primitive collision to read.
	 * @return The currently assigned collision profile name (e.g., "Custom", "NoCollision"). 
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static FName GetCollisionProfileName(const FPrimitiveCollision& Collision)
	{
		return Collision.GetProfileName();
	}

	/** 
	 * Updates the collision enabled state (Query, Physics, or both). Implicitly sets ProfileName to "Custom".
	 * @param Collision The primitive collision to modify.
	 * @param InEnabled The new enabled state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionEnabled(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionEnabled::Type InEnabled)
	{
		Collision.SetCollisionEnabled(InEnabled);
	}

	/** 
	 * @param Collision The primitive collision to read.
	 * @return The current collision enabled state. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static ECollisionEnabled::Type GetCollisionEnabled(const FPrimitiveCollision& Collision)
	{
		return Collision.GetCollisionEnabled();
	}

	/** 
	 * Sets the object type for this collision. Implicitly sets ProfileName to "Custom".
	 * @param Collision The primitive collision to modify.
	 * @param InObjectType Type to change to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionObjectType(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionChannel InObjectType)
	{
		Collision.SetObjectType(InObjectType);
	}

	/** 
	 * @param Collision The primitive collision to read.
	 * @return The logical object type channel. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static ECollisionChannel GetCollisionObjectType(const FPrimitiveCollision& Collision)
	{
		return Collision.GetObjectType();
	}

	/** 
	 * Sets all collision channels to a specific response. Implicitly sets ProfileName to "Custom".
	 * @param Collision The primitive collision to modify.
	 * @param InResponse Response to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionResponseToAllChannels(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionResponse InResponse)
	{
		Collision.SetAllResponses(InResponse);
	}

	/** 
	 * Sets the response for a specific collision channel. Implicitly sets ProfileName to "Custom".
	 * @param Collision The primitive collision to modify.
	 * @param InResponse Response to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionResponseToChannel(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionChannel InChannel, const ECollisionResponse InResponse)
	{
		Collision.SetResponse(InChannel, InResponse);
	}

	/** 
	 * Retrieves the response for a specific collision channel. 
	 * @param Collision The primitive collision to read.
	 * @param InChannel Channel to find.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static ECollisionResponse GetCollisionResponseToChannel(const FPrimitiveCollision& Collision, const ECollisionChannel InChannel)
	{
		return Collision.GetResponse(InChannel);
	}

	/** 
	 * Captures the current collision state of any Primitive Component into a serializable struct.
	 * @param OutData The struct to receive the captured collision state.
	 * @param Target The source component (e.g., StaticMesh, Capsule, Box).
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void GetPrimitiveCollision(FPrimitiveCollision& OutData, const UPrimitiveComponent* Target)
	{
		OutData.FromPrimitiveComponent(Target);
	}

	/** 
	 * Applies a collision configuration struct to a Primitive Component.
	 * @param Target The component to update.
	 * @param Collision The struct containing the collision settings to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetPrimitiveCollision(UPrimitiveComponent* Target, UPARAM(ref) FPrimitiveCollision& Collision)
	{
		Collision.ToPrimitiveComponent(Target);
	}
};