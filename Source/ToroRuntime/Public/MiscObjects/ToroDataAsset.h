// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Engine/DataAsset.h"
#include "Misc/DataValidation.h"
#include "ToroDataAsset.generated.h"

/**
 * Base class for data assets with editor-integrated validation.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TORORUNTIME_API UToroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UToroDataAsset();

#if WITH_EDITOR
protected:
	/**
	 * Generic function to validate objects during changelist validations, etc.
	 * @param	Context	the context holding validation warnings/errors.
	 * @return Valid if this object has data validation rules set up for it and the data for this object is valid.
	 *         Returns Invalid if it does not pass the rules. Returns NotValidated if no rules are set for this object.
	 */
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
