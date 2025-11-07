// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Factories/Factory.h"
#include "ToroGraphFactory.generated.h"

UCLASS()
class UToroGraphFactory final : public UFactory
{
	GENERATED_BODY()

public:

	UToroGraphFactory();

	virtual bool ConfigureProperties() override;
	virtual FString GetDefaultNewAssetName() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
		EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

private:

	TSubclassOf<class UToroDataGraph> GraphClass;
};
