// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroClassCustomization.h"
#include "Actors/ToroCharacter.h"
#include "Actors/ToroVolume.h"
#include "Actors/ToroActor.h"

class TOROEDITOR_API FToroActorCustomization : public FToroClassCustomization
{
protected:
};

class TOROEDITOR_API FToroVolumeCustomization : public FToroClassCustomization
{
protected:
};

class TOROEDITOR_API FToroCharacterCustomization : public FToroClassCustomization
{
protected:
	virtual TArray<FString> GetShowOnlyCategories() override
	{
		return TArray<FString>{
			TEXT("Settings"),
			TEXT("Tools"),
			TEXT("Lighting"),
			TEXT("Rendering"),
			// TEXT("HLOD"),
			// TEXT("Mobile"),
			// TEXT("RayTracing"),
			TEXT("Pawn"),
			// TEXT("Replication"),
			// TEXT("Networking"),
			TEXT("Input"),
			TEXT("Actor"),
			TEXT("Optimization"),
			TEXT("LevelOfDetail"),
			TEXT("MaterialParameters"),
			TEXT("TextureStreaming"),
			TEXT("WorldPartition"),
			TEXT("LevelInstance"),
			TEXT("DataLayers")
		};
	}
};