// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "ToroEventPayload.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroEventPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = EventPayload)
		TObjectPtr<UObject> Invoker;

	UPROPERTY(BlueprintReadOnly, Category = EventPayload, meta = (ExposeOnSpawn = true))
		TMap<FName, FString> Strings;

	UPROPERTY(BlueprintReadOnly, Category = EventPayload, meta = (ExposeOnSpawn = true))
		TMap<FName, float> Floats;

	UFUNCTION(BlueprintCallable, Category = EventPayload)
		void AddString(const FName Key, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = EventPayload)
		void AddFloat(const FName Key, const float Value);

	UFUNCTION(BlueprintPure, Category = EventPayload)
		FString GetString(const FName Key) const;

	UFUNCTION(BlueprintPure, Category = EventPayload)
		float GetFloat(const FName Key) const;
};
