// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Helpers/GameplayTagMacros.h"
#include "Interfaces/CharInterface.h"
#include "Interfaces/GuidInterface.h"
#include "ToroCharacter.generated.h"

namespace CharacterTags
{
	inline const FName NearInteractable = FName("Near_Interactable");
	
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Character)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Player)

	TOROCORE_API CREATE_TAG_VERIFIER(Character)
	TOROCORE_API inline bool IsPlayer(const AActor* Target)
	{
		return ICharInterface::GetCharacterID(Target) == TAG_Player.GetTag();
	}
}

UCLASS(Abstract, PrioritizeCategories = Settings)
class TOROCORE_API AToroCharacter : public ACharacter, public ICharInterface, public IGuidInterface
{
	GENERATED_BODY()
#if WITH_EDITOR
	friend class FToroCharacterDetails;
#endif
public:

	AToroCharacter() {}

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject", DeterminesOutputType = "Class"))
	static AToroCharacter* FindCharacter(const UObject* ContextObject, const TSubclassOf<AToroCharacter> Class,
		UPARAM(meta = (Categories = "Character")) const FGameplayTag CharID);

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void Teleport(const FVector& InLocation, const FRotator& InRotation);

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void ChangeCharacterID(UPARAM(meta = (Categories = "Character")) const FGameplayTag InNewID);

	virtual FGuid GetUniqueGUID_Implementation() override { return UniqueGuid; }
	virtual FGameplayTag GetCharacterID_Implementation() const override { return CharacterID; }

	virtual bool GetFocusPoint_Implementation(FVector& Location) const override;
	virtual bool GetViewTarget_Implementation(FVector& Location) const override;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override;

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (DisplayPriority = -10, Categories = "Character"))
		FGameplayTag CharacterID;

	UPROPERTY(EditAnywhere, Category = Actor, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid UniqueGuid;

	virtual void OnConstruction(const FTransform& Transform) override;
};
