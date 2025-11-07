// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDelegates.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FToroBoolDelegate, const bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToroBoolDelegateBP, const bool, bState);

USTRUCT(meta = (Hidden))
struct FToroDelegatesDummyStruct
{
	GENERATED_BODY()
};