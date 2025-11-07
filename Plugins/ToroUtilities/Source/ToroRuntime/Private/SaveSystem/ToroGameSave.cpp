// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroGameSave.h"

UToroGameSave::UToroGameSave(): PlayTime(0.0f), Sequence({0})
{
	bSingleSlot = false;
}

void UToroGameSave::DeleteData()
{
	Super::DeleteData();
	PlayTime = 0.0f;
	Sequence.Empty();
	Archives.Empty();
	Items.Empty();
	Equipment.Reset();
}

void UToroGameSave::SerializeData(FArchive& Ar)
{
	Ar << PlayTime;
	Ar << Sequence;
	Ar << Archives;
	Ar << Items;
	FArchiveUObject::SerializeSoftObjectPath(Ar, Equipment);
}
