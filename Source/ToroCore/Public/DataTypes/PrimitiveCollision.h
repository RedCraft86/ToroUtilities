// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
#if WITH_EDITOR
	friend class FPrimitiveCollisionDetails;
#endif

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

	struct TOROCORE_API Utils
	{
		/** 
		 * Retrieves raw collision data for a named profile from the engine. 
		 * @param ProfileName The name of the profile to search for.
		 * @param OutData (Optional) Pointer to receive the template data if found.
		 * @return True if the profile exists.
		 */
		static bool GetProfile(const FName& ProfileName, FCollisionResponseTemplate* OutData = nullptr);

		/** 
		 * Iterates through all available collision channels.
		 * @param Func The callback function to execute for each channel.
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
	 * @param Collision The collision struct to update (Pass-by-Reference).
	 * @param ProfileName The name of the project collision preset to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionProfileName(UPARAM(ref) FPrimitiveCollision& Collision, const FName ProfileName)
	{
		Collision.SetProfileName(ProfileName);
	}

	/** @return The currently assigned collision profile name (e.g., "Custom", "NoCollision"). */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static FName GetCollisionProfileName(const FPrimitiveCollision& Collision)
	{
		return Collision.GetProfileName();
	}

	/** 
	 * Updates the collision enabled state (Query, Physics, or both).
	 * @param Collision The collision struct to update.
	 * @param InEnabled The new enabled state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionEnabled(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionEnabled::Type InEnabled)
	{
		Collision.SetCollisionEnabled(InEnabled);
	}

	/** @return The current collision enabled state. */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static ECollisionEnabled::Type GetCollisionEnabled(const FPrimitiveCollision& Collision)
	{
		return Collision.GetCollisionEnabled();
	}

	/** 
	 * Sets the object type for this collision. Only relevant if ProfileName is "Custom".
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionObjectType(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionChannel InObjectType)
	{
		Collision.SetObjectType(InObjectType);
	}

	/** @return The logical object type channel (e.g., WorldStatic, Pawn). */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static ECollisionChannel GetCollisionObjectType(const FPrimitiveCollision& Collision)
	{
		return Collision.GetObjectType();
	}

	/** 
	 * Sets all collision channels to a specific response (Ignore, Overlap, or Block). 
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionResponseToAllChannels(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionResponse InResponse)
	{
		Collision.SetAllResponses(InResponse);
	}

	/** 
	 * Sets the response for a specific collision channel. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static void SetCollisionResponseToChannel(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionChannel InChannel, const ECollisionResponse InResponse)
	{
		Collision.SetResponse(InChannel, InResponse);
	}

	/** 
	 * Retrieves the response for a specific collision channel. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|PrimitiveCollision")
	static ECollisionResponse GetCollisionResponseToChannel(UPARAM(ref) FPrimitiveCollision& Collision, const ECollisionChannel InChannel)
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