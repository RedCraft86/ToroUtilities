// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"

#define SETTING_GETTER(Class) static Class* Get() \
	{ \
		return GetMutableDefault<Class>(); \
	}

#define ENGINE_SUBSYSTEM_GETTER(Class) static Class* Get() \
	{ \
		return GEngine ? GEngine->GetEngineSubsystem<Class>() : nullptr; \
	}

#define EDITOR_SUBSYSTEM_GETTER(Class) static Class* Get() \
	{ \
		return GEditor ? GEditor->GetEditorSubsystem<Class>() : nullptr; \
	}

#define LOCAL_PLAYER_SUBSYSTEM_GETTER(Class) static Class* Get(const UObject* ContextObject, const int PlayerIdx = 0) \
	{ \
		const APlayerController* Controller = UGameplayStatics::GetPlayerController(FWorldGetter::Get(ContextObject), PlayerIdx); \
		const ULocalPlayer* LocalPlayer = IsValid(Controller) ? Controller->GetLocalPlayer() : nullptr; \
		return IsValid(LocalPlayer) ? LocalPlayer->GetSubsystem<Class>() : nullptr; \
	}

#define GAME_INSTANCE_SUBSYSTEM_GETTER(Class) static Class* Get(const UObject* ContextObject) \
	{ \
		const UWorld* World = FWorldGetter::Get(ContextObject); \
		const UGameInstance* GI = IsValid(World) ? World->GetGameInstance() : nullptr; \
		return IsValid(GI) ? GI->GetSubsystem<Class>() : nullptr; \
	}

#define WORLD_SUBSYSTEM_GETTER(Class) static Class* Get(const UObject* ContextObject) \
	{ \
		const UWorld* World = FWorldGetter::Get(ContextObject); \
		return IsValid(World) ? World->GetSubsystem<Class>() : nullptr; \
	}

#define GLOBAL_CLASS_GETTER(Class, Func) template<typename T = Class> \
	static T* Get(const UObject* ContextObject) \
	{ \
		return Cast<T>(UGameplayStatics::Func(FWorldGetter::Get(ContextObject))); \
	}

#define PLAYER_CLASS_GETTER(Class, Func) template<typename T = Class> \
	static T* Get(const UObject* ContextObject, const int32 PlayerIdx = 0) \
	{ \
		return Cast<T>(UGameplayStatics::Func(FWorldGetter::Get(ContextObject), PlayerIdx)); \
	}

#define GLOBAL_COMPONENT_GETTER(Class, OwnerClass, Property) template<typename T = Class> \
	static T* Get(const UObject* ContextObject) \
	{ \
		const OwnerClass* Owner = OwnerClass::Get(FWorldGetter::Get(ContextObject)); \
		return Owner ? Owner->Property : nullptr; \
}

#define PLAYER_COMPONENT_GETTER(Class, OwnerClass, Property) template<typename T = Class> \
	static T* Get(const UObject* ContextObject, const int32 PlayerIdx = 0) \
	{ \
		const OwnerClass* Owner = OwnerClass::Get<OwnerClass>(FWorldGetter::Get(ContextObject), PlayerIdx); \
		return Owner ? Owner->Property : nullptr; \
	}
