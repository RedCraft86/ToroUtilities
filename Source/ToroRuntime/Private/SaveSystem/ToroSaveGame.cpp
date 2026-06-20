// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0). 
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "SaveSystem/ToroSaveGame.h"
#include "SaveSystem/ToroSaveArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Libraries/ToroOodleLibrary.h"
#include "Helpers/WorldGetter.h"
#include "Misc/FileHelper.h"
#include "ToroRuntime.h"
#if WITH_EDITOR
#include "Misc/App.h"
#include "Engine/Engine.h"
#else
#include "Libraries/ToroWindowsLibrary.h"
#endif

EToroSaveLoadStatus HandleError(const EToroSaveLoadStatus Status, const FName& SaveName)
{
	if (SaveName.IsNone())
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Asked to handle SaveGame errors with no name!"))
	}

	if (Status == EToroSaveLoadStatus::Waiting || Status == EToroSaveLoadStatus::Succeeded)
	{
		return Status;
	}

	FString StatusName;
	switch (Status)
	{
		case EToroSaveLoadStatus::CompressFailed:	StatusName = TEXT("Compression Failed");	break;
		case EToroSaveLoadStatus::DecompressFailed:	StatusName = TEXT("Decompression Failed");	break;
		case EToroSaveLoadStatus::FileWriteFailed:	StatusName = TEXT("File-Write Failed");		break;
		case EToroSaveLoadStatus::FileReadFailed:	StatusName = TEXT("File-Read Failed");		break;
		case EToroSaveLoadStatus::SerializeFailed:	StatusName = TEXT("Serialization Failed");	break;
		default: StatusName = TEXT("Unknown Error"); break;
	}

#if !WITH_EDITOR
	UToroWindowsLibrary::OpenDialogue(TEXT("Save File Error!"), 
		FString::Printf(TEXT("Save: %s\nStatus: %s"), *SaveName.ToString(), *StatusName), 
		EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error
	);
#else
	UE_LOG(LogToroRuntime, Error, 
		TEXT("Failed to save/load '%s' because %s"), 
		*SaveName.ToString(), *StatusName)
#endif

	return Status;
}

FString UToroSaveGame::GetSavePath(const uint8 Slot) const
{
	const FString SlotPath = (Slot == 0) ? FString() : FString::Printf(TEXT("Slot_%02d"), Slot);
	return FPaths::ProjectSavedDir() / SlotPath / SaveName.ToString() + TEXT(".sar");
}

UE5Coro::TCoroutine<EToroSaveLoadStatus> UToroSaveGame::SaveToFile(const uint8 Slot)
{
	if (CurrentOperation != EToroSaveOperation::None)
	{
		co_return EToroSaveLoadStatus::Waiting;
	}

	// Pin this object for the duration of the operation
	TStrongObjectPtr<UToroSaveGame> KeepAlive(this);

	CurrentOperation = EToroSaveOperation::Saving;
	const FString FilePath(GetSavePath(Slot));

	TArray<uint8> UncompressedData;
	{
		FMemoryWriter Writer(UncompressedData, true);
		FToroSaveGameProxyArchive<false> ProxyAr(Writer);
		FBinaryArchiveFormatter Formatter(ProxyAr);
		FStructuredArchive Ar(Formatter);

		FStructuredArchive::FRecord RootRecord = Ar.Open().EnterRecord();
		if (bSerializeSelf)
		{
			SerializeScriptProperties(RootRecord.EnterField(SelfFieldName));
		}

		SerializeData(RootRecord);
		Ar.Close();

		if (ProxyAr.IsError() || Writer.IsError())
		{
			CurrentOperation = EToroSaveOperation::None;
			co_return HandleError(EToroSaveLoadStatus::SerializeFailed, SaveName);
		}
	}

	co_await UE5Coro::Async::MoveToTask();

	EToroSaveLoadStatus FileWriteResult = EToroSaveLoadStatus::Succeeded;

	TArray<uint8> CompressedData;
	if (UToroOodleLibrary::OodleCompress(UncompressedData, CompressedData))
	{
		if (!FFileHelper::SaveArrayToFile(MoveTemp(CompressedData), *FilePath))
		{
			FileWriteResult = EToroSaveLoadStatus::FileWriteFailed;
		}
	}
	else
	{
		FileWriteResult = EToroSaveLoadStatus::CompressFailed;
	}

	co_await UE5Coro::Async::MoveToGameThread();

	CurrentOperation = EToroSaveOperation::None;
	co_return HandleError(FileWriteResult, SaveName);
}
}

UE5Coro::TCoroutine<EToroSaveLoadStatus> UToroSaveGame::LoadFromFile(const uint8 Slot)
{
	if (CurrentOperation != EToroSaveOperation::None)
	{
		co_return EToroSaveLoadStatus::Waiting;
	}

	// Pin this object for the duration of the operation
	TStrongObjectPtr<UToroSaveGame> KeepAlive(this);

	CurrentOperation = EToroSaveOperation::Loading;
	const FString FilePath(GetSavePath(Slot));

	co_await UE5Coro::Async::MoveToTask();

	EToroSaveLoadStatus FileReadResult = EToroSaveLoadStatus::Succeeded;

	TArray<uint8> CompressedData;
	TArray<uint8> UncompressedData;
	if (FFileHelper::LoadFileToArray(CompressedData, *FilePath))
	{
		if (!UToroOodleLibrary::OodleDecompress(CompressedData, UncompressedData))
		{
			FileReadResult = EToroSaveLoadStatus::DecompressFailed;
		}
	}
	else
	{
		FileReadResult = EToroSaveLoadStatus::FileReadFailed;
	}

	co_await UE5Coro::Async::MoveToGameThread();

	if (FileReadResult != EToroSaveLoadStatus::Succeeded)
	{
		CurrentOperation = EToroSaveOperation::None;
		co_return HandleError(FileReadResult, SaveName);
	}

	bool bSerializeError = false;
	{
		FMemoryReader Reader(UncompressedData, true);
		FToroSaveGameProxyArchive<true> ProxyAr(Reader);
		FBinaryArchiveFormatter Formatter(ProxyAr);
		FStructuredArchive Ar(Formatter);

		FStructuredArchive::FRecord RootRecord = Ar.Open().EnterRecord();
		if (bSerializeSelf)
		{
			SerializeScriptProperties(RootRecord.EnterField(SelfFieldName));
		}

		SerializeData(RootRecord);
		Ar.Close();

		bSerializeError = ProxyAr.IsError() || Reader.IsError();
	}

	CurrentOperation = EToroSaveOperation::None;
	co_return HandleError(bSerializeError
		? EToroSaveLoadStatus::SerializeFailed 
		: EToroSaveLoadStatus::Succeeded, 
		SaveName
	);
}

UWorld* UToroSaveGame::GetWorld() const
{
#if WITH_EDITOR
	return FApp::IsGame() ? FWorldGetter::Get(this) : GEngine->GetCurrentPlayWorld();
#else
	return FWorldGetter::Get(this);
#endif
}
