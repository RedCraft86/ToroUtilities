// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PrimitiveCollision.generated.h"

/**
 * Stores either a named collision profile or custom channel responses for a primitive component.
 * Editing individual settings switches to the custom profile.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FPrimitiveCollision final
{
	GENERATED_BODY()

private:
#if WITH_EDITOR
	// TODO: friend class FPrimitiveCollisionDetails
#endif

	/**
	 * Selected collision profile name; custom edits switch this to Custom.
	 */
	UPROPERTY(BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		FName ProfileName;

	/**
	 * Enabled query and physics collision modes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;

	/**
	 * Object channel used when the settings are custom.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = true))
		TEnumAsByte<ECollisionChannel> ObjectType;

	/**
	 * Response for each channel when the settings are custom.
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
		FillResponses();
	}

	FPrimitiveCollision(const FName& ProfileName)
		: CollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics)
		, ObjectType(ECC_WorldStatic)
		, Responses({})
	{
		SetProfileName(ProfileName);
	}

	bool operator==(const FPrimitiveCollision& Other) const { return Equals(Other); }
	bool operator!=(const FPrimitiveCollision& Other) const { return !Equals(Other); }

	/**
	 * Compares enabled mode, object type, and channel responses; profile names are ignored.
	 * @param Other Settings to compare against.
	 * @return True if enabled mode, object type, and channel responses match.
	 */
	bool Equals(const FPrimitiveCollision& Other) const;

	/**
	 * Returns whether the stored name resolves to a registered collision profile.
	 * @return whether the stored name resolves to a registered collision profile.
	 */
	bool HasValidProfileName() const;

	/**
	 * Loads a named profile, or selects Custom and returns false for an unknown name.
	 * @param InName Collision profile name to apply.
	 * @return True if the named profile was found; otherwise false.
	 */
	bool SetProfileName(const FName& InName);

	/**
	 * @return the stored profile name.
	 */
	const FName& GetProfileName() const { return ProfileName; }

	/**
	 * Sets the collision mode and selects Custom.
	 * @param InEnabled Collision mode to assign.
	 */
	void SetCollisionEnabled(const ECollisionEnabled::Type InEnabled);

	/**
	 * @return the stored collision mode.
	 */
	ECollisionEnabled::Type GetCollisionEnabled() const { return CollisionEnabled; }

	/**
	 * Sets the object type and selects Custom.
	 * @param InObjectType Object collision channel to assign.
	 */
	void SetObjectType(const ECollisionChannel InObjectType);

	/**
	 * @return the stored object type.
	 */
	ECollisionChannel GetObjectType() const { return ObjectType; }

	/**
	 * Sets one channel response and selects Custom.
	 * @param InChannel Collision channel to access.
	 * @param InResponse Collision response to assign.
	 */
	void SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse);

	/**
	 * Returns the stored response for a channel; missing channels return Ignore.
	 * @param InChannel Collision channel to access.
	 * @return the stored response for a channel; missing channels return Ignore.
	 */
	ECollisionResponse GetResponse(const ECollisionChannel InChannel) const { return Responses.FindRef(InChannel); }

	/**
	 * Sets every channel response and selects Custom.
	 * @param InResponse Collision response to assign.
	 */
	void SetAllResponses(const ECollisionResponse InResponse);

	/**
	 * Fills missing channel responses and selects Custom.
	 * @param InResponse Collision response to assign.
	 */
	void FillResponses(const ECollisionResponse InResponse = ECR_Ignore);

	/**
	 * Replaces the response map and selects Custom.
	 * @param InResponses Channel responses to assign.
	 */
	void SetResponses(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses);

	/**
	 * @return the stored response map.
	 */
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& GetResponses() const { return Responses; }

	/**
	 * Reads the collision profile or custom fields from Target when valid.
	 * @param Target Component to read.
	 */
	void FromPrimitiveComponent(const UPrimitiveComponent* Target);

	/**
	 * Applies the named profile or custom fields to Target when valid.
	 * @param Target Component to update.
	 */
	void ToPrimitiveComponent(UPrimitiveComponent* Target) const;

	/**
	 * Provides collision profile lookup and channel enumeration helpers.
	 */
	struct TORORUNTIME_API Utils final
	{
		/**
		 * Looks up a named collision profile and optionally writes its template.
		 * @param ProfileName Collision profile name to load or assign.
		 * @param OutData Receives the settings read from the component.
		 * @return True if the named profile was found.
		 */
		static bool GetProfileData(const FName& ProfileName, FCollisionResponseTemplate* OutData = nullptr);

		/**
		 * Invokes Func for each collision channel, optionally skipping hidden editor entries.
		 * @param Func Callback invoked for each collision channel.
		 * @param bIgnoreHidden Whether to skip channels hidden in the editor.
		 */
		static void ForEachChannel(const TFunction<void(const ECollisionChannel, const int32, const UEnum*)>& Func, const bool bIgnoreHidden);
	};
};

/**
 * Blueprint-accessible operations for FPrimitiveCollision.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UPrimitiveCollisionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Sets the collision profile name on Settings.
	 * @param Settings Settings to apply to the component.
	 * @param ProfileName Collision profile name to load or assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionProfileName(UPARAM(ref) FPrimitiveCollision& Settings, const FName ProfileName);

	/**
	 * Returns the collision profile name stored in Settings.
	 * @param Settings Settings to inspect.
	 * @return the collision profile name stored in Settings.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static FName GetCollisionProfileName(const FPrimitiveCollision& Settings);

	/**
	 * Sets the collision mode on Settings.
	 * @param Settings Settings to apply to the component.
	 * @param InEnabled Collision mode to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionEnabled(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionEnabled::Type InEnabled);

	/**
	 * Returns the collision mode stored in Settings.
	 * @param Settings Settings to inspect.
	 * @return the collision mode stored in Settings.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static ECollisionEnabled::Type GetCollisionEnabled(const FPrimitiveCollision& Settings);

	/**
	 * Sets the object collision channel on Settings.
	 * @param Settings Settings to apply to the component.
	 * @param InObjectType Object collision channel to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionObjectType(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionChannel InObjectType);

	/**
	 * Returns the object collision channel stored in Settings.
	 * @param Settings Settings to inspect.
	 * @return the object collision channel stored in Settings.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static ECollisionChannel GetCollisionObjectType(const FPrimitiveCollision& Settings);

	/**
	 * Sets every channel response on Settings.
	 * @param Settings Settings to apply to the component.
	 * @param InResponse Collision response to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionResponseToAllChannels(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionResponse InResponse);

	/**
	 * Sets the response for InChannel on Settings.
	 * @param Settings Settings to apply to the component.
	 * @param InChannel Collision channel to access.
	 * @param InResponse Collision response to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionResponseToChannel(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionChannel InChannel, const ECollisionResponse InResponse);

	/**
	 * Returns the response for InChannel stored in Settings.
	 * @param Settings Settings to inspect.
	 * @param InChannel Collision channel to access.
	 * @return the response for InChannel stored in Settings.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static ECollisionResponse GetCollisionResponseToChannel(const FPrimitiveCollision& Settings, const ECollisionChannel InChannel);

	/**
	 * Reads collision settings from the target component into OutData.
	 * @param OutData Receives the settings read from the component.
	 * @param Target Component to read.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void GetPrimitiveCollision(FPrimitiveCollision& OutData, const UPrimitiveComponent* Target);

	/**
	 * Applies collision settings to the target component.
	 * @param Target Component to update.
	 * @param Settings Settings to apply to the component.
	 */
	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetPrimitiveCollision(UPrimitiveComponent* Target, UPARAM(ref) FPrimitiveCollision& Settings);
};