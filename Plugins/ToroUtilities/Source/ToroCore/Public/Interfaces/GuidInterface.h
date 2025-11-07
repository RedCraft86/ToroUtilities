// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "GuidInterface.generated.h"

UINTERFACE()
class UGuidInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API IGuidInterface
{
	GENERATED_BODY()

public:

	/* Universal generic Guid getter */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = GuidInterface)
		FGuid GetUniqueGUID();
	virtual FGuid GetUniqueGUID_Implementation() { return FGuid(); }

	static FGuid GetUniqueGUID(UObject* Target)
	{
		return IsValid(Target) && Target->Implements<UGuidInterface>()
			? Execute_GetUniqueGUID(Target) : FGuid();
	}
};