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

	bool Equals(const FPrimitiveCollision& Other) const;
	bool operator==(const FPrimitiveCollision& Other) const { return Equals(Other); }
	bool operator!=(const FPrimitiveCollision& Other) const { return !Equals(Other); }

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
	void FillResponses(const ECollisionResponse InResponse = ECR_Ignore);

	void SetResponses(const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses);
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& GetResponses() const { return Responses; }

	void FromPrimitiveComponent(const UPrimitiveComponent* Target);
	void ToPrimitiveComponent(UPrimitiveComponent* Target) const;

	struct TORORUNTIME_API Utils final
	{
		static bool GetProfileData(const FName& ProfileName, FCollisionResponseTemplate* OutData = nullptr);
		static void ForEachChannel(const TFunction<void(const ECollisionChannel, const int32, const UEnum*)>& Func, const bool bIgnoreHidden);
	};
};

/**
 * Exposes collision settings access and component transfer to Blueprints.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UPrimitiveCollisionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionProfileName(UPARAM(ref) FPrimitiveCollision& Settings, const FName ProfileName);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static FName GetCollisionProfileName(const FPrimitiveCollision& Settings);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionEnabled(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionEnabled::Type InEnabled);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static ECollisionEnabled::Type GetCollisionEnabled(const FPrimitiveCollision& Settings);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionObjectType(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionChannel InObjectType);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static ECollisionChannel GetCollisionObjectType(const FPrimitiveCollision& Settings);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionResponseToAllChannels(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionResponse InResponse);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetCollisionResponseToChannel(UPARAM(ref) FPrimitiveCollision& Settings, const ECollisionChannel InChannel, const ECollisionResponse InResponse);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static ECollisionResponse GetCollisionResponseToChannel(const FPrimitiveCollision& Settings, const ECollisionChannel InChannel);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void GetPrimitiveCollision(FPrimitiveCollision& OutData, const UPrimitiveComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Collision|Primitive")
		static void SetPrimitiveCollision(UPrimitiveComponent* Target, UPARAM(ref) FPrimitiveCollision& Settings);
};