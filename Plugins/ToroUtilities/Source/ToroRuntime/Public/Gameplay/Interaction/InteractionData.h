// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerCharacter.h"
#include "InteractionData.generated.h"

USTRUCT(BlueprintType)
struct TORORUNTIME_API FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (MultiLine = true))
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		TObjectPtr<UTexture2D> Icon;

	FInteractionInfo(): bEnabled(true), Label(INVTEXT("Interact")), Icon(nullptr) {}
	FInteractionInfo(const bool bInEnabled, const FText& InLabel)
		: bEnabled(bInEnabled), Label(InLabel), Icon(nullptr)
	{}

	static FInteractionInfo Empty;
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInteractionCache
{
	GENERATED_BODY()

private:

	bool bTriggered;
	TWeakObjectPtr<AActor> Target;

public:

	FInteractionCache(): bTriggered(false), Target(nullptr) {}

	void Reset();

	void SetTarget(AActor* InTarget);
	AActor* GetTarget() const { return Target.Get(); }

	void StopInteract(AToroPlayerCharacter* Player);
	void StartInteract(AToroPlayerCharacter* Player, const FHitResult& Hit);
	FInteractionInfo GetInteractInfo(const FHitResult& Hit) const;
};