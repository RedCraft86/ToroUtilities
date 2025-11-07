// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveTypes.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Serialization/BufferArchive.h"
#include "Helpers/DataCompression.h"
#include "Helpers/WindowsHelpers.h"
#include "ToroRuntime.h"

#define LOG_ERROR(Msg, Data) WindowsHelpers::OpenDialogue(TEXT("Save System Error"), \
	FString::Printf(TEXT("%s\n%s"), TEXT(Msg), *GetSavePath()), EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error); \
	UE_LOG(LogToroRuntime, Error, TEXT("%s [Size: %d] [Path: %s]"), TEXT(Msg), Data.Num(), *GetSavePath())

UToroSaveObject::UToroSaveObject(): bSingleSlot(false), Slot(0), Status(ESaveGameStatus::None)
{
	UToroSaveObject::ValidateFileName();
}

UToroSaveObject* UToroSaveObject::Create(UToroSaveManager* InOwner, const TSubclassOf<UToroSaveObject>& InClass, const uint8 InSlot)
{
	if (!InOwner || !InClass)
	{
		return nullptr;
	}

	if (UToroSaveObject* Obj = NewObject<UToroSaveObject>(InOwner, *InClass))
	{
		Obj->Slot = Obj->bSingleSlot ? 0 : InSlot;
		Obj->ValidateFileName();

		Obj->Manager = InOwner;
		Obj->LoadObject(nullptr);
		
		UE_LOG(LogToroRuntime, Display, TEXT("Created Save Object for %s #%02d"), *Obj->SaveName, Obj->Slot);
		return Obj;
	}

	return nullptr;
}

void UToroSaveObject::DeleteData()
{
	UpdateActivity(ESaveGameActivity::Deleting);
	IFileManager::Get().Delete(*GetSavePath());
}

void UToroSaveObject::SaveObject(const TFunction<void(const ESaveGameStatus)>& Callback)
{
	UpdateActivity(ESaveGameActivity::Saving);

	FBufferArchive ToBinary(true);
	SerializeData(ToBinary);

	if (TArray<uint8> Compressed; DataCompression::OodleCompress(ToBinary, Compressed))
	{
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this, Callback, Compressed = MoveTemp(Compressed)]()
		{
			if (FFileHelper::SaveArrayToFile(Compressed, *GetSavePath()))
			{
				UpdateStatus(ESaveGameStatus::Success, Callback);
			}
			else
			{
				LOG_ERROR("Failed to write save file!", Compressed)
				UpdateStatus(ESaveGameStatus::FileWriteError, Callback);
			}
		});
	}
	else
	{
		LOG_ERROR("Failed to compress save data!", ToBinary)
		UpdateStatus(ESaveGameStatus::CompressError, Callback);
	}

	ToBinary.FlushCache();
	ToBinary.Close();
}

void UToroSaveObject::LoadObject(const TFunction<void(const ESaveGameStatus)>& Callback)
{
	if (!FPaths::FileExists(GetSavePath()))
	{
		SaveObject(Callback);
		return;
	}
	
	UpdateActivity(ESaveGameActivity::Loading);

	AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [this, Callback]()
	{
		TArray<uint8> Compressed;
		if (!FFileHelper::LoadFileToArray(Compressed, *GetSavePath()))
		{
			LOG_ERROR("Failed to read save file!", Compressed);
			UpdateStatus(ESaveGameStatus::FileReadError, Callback);
			return;
		}

		// Assume this is not a loading error and there's just no data to load
		if (Compressed.IsEmpty())
		{
			UpdateStatus(ESaveGameStatus::Success, Callback);
			return;
		}

		AsyncTask(ENamedThreads::GameThread, [this, Callback, Compressed = MoveTemp(Compressed)]()
		{
			if (TArray<uint8> RawData; DataCompression::OodleDecompress(Compressed, RawData))
			{
				FMemoryReader FromBinary(RawData, true);
				FromBinary.Seek(0);

				SerializeData(FromBinary);

				FromBinary.FlushCache();
				FromBinary.Close();

				UpdateStatus(ESaveGameStatus::Success, Callback);
			}
			else
			{
				LOG_ERROR("Failed to decompress save data!", Compressed)
				UpdateStatus(ESaveGameStatus::DecompressError, Callback);
			}
		});
	});
}

FString UToroSaveObject::GetSavePath()
{
	FString BasePath = FPaths::ProjectSavedDir() / TEXT("SaveGames");

	if (const FName DemoName = UToroSettings::Get()->DemoName; !DemoName.IsNone())
	{
		BasePath /= DemoName.ToString();
	}
	
	if (!bSingleSlot)
	{
		BasePath /= Slot == 0 ? TEXT("AutoSave") : FString::Printf(TEXT("Slot_%02d"), Slot);
	}

	return BasePath / (SaveName + TEXT(".tsave"));
}

void UToroSaveObject::UpdateActivity(const ESaveGameActivity InActivity) const
{
	if (Manager) Manager->OnActivity(this, InActivity);
}

void UToroSaveObject::UpdateStatus(const ESaveGameStatus InStatus, const TFunction<void(const ESaveGameStatus)>& Func)
{
	if (IsInGameThread())
	{
		Status = InStatus;
		if (Func) Func(Status);
		return;
	}
	AsyncTask(ENamedThreads::GameThread, [this, Func, InStatus]()
	{
		Status = InStatus;
		if (Func) Func(Status);
	});
}

void UToroSaveObject::ValidateFileName()
{
	if (SaveName.IsEmpty()) SaveName = GetClass()->GetName();
	SaveName = FPaths::MakeValidFileName(SaveName).Left(64);
}

#if WITH_EDITOR
void UToroSaveObject::PostInitProperties()
{
	Super::PostInitProperties();
	ValidateFileName();
}

void UToroSaveObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UToroSaveObject, SaveName))
	{
		ValidateFileName();
	}
}
#endif