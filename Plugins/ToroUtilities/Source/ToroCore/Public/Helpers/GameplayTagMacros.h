// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"

#define CREATE_TAG_VERIFIER(BaseTag) \
	inline bool IsValidTag(const FGameplayTag& InTag) \
	{ \
		static FGameplayTag BaseTag = TAG_##BaseTag.GetTag(); \
		return InTag.MatchesTag(BaseTag) && InTag != BaseTag; \
	}

#define DECLARE_GAMEPLAY_TAG(TagName) UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_##TagName)
#define DEFINE_GAMEPLAY_TAG(TagName) UE_DEFINE_GAMEPLAY_TAG(TAG_##TagName, #TagName)
#define DEFINE_GAMEPLAY_TAG_CHILD(BaseName, TagName) \
		UE_DEFINE_GAMEPLAY_TAG(TAG_##TagName, UE_INLINE_STRINGIFY(BaseName.TagName))