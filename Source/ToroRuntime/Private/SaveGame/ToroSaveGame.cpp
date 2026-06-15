// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "SaveGame/ToroSaveGame.h"
#include "Libraries/ToroOodleLibrary.h"
#include "SaveGame/ToroSaveArchive.h"
#include "Misc/FileHelper.h"
#include "ToroGlobals.h"

#if ALLOW_PACKAGED_FEATURES
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

#if ALLOW_PACKAGED_FEATURES
	UToroWindowsLibrary::OpenDialogue(TEXT("Save File Error!"), 
		FString::Printf(TEXT("Save: %s\nStatus: %s"), *SaveName, *StatusName), 
		EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error
	);
#else
	UE_LOG(LogToroRuntime, Error, TEXT("Failed to save/load '%s' because %s"), *SaveName.ToString(), *StatusName)
#endif

	return Status;
}

UE5Coro::TCoroutine<EToroSaveLoadStatus> UToroSaveGame::SaveToFile(const uint8 Slot)
{
	if (Operation != EToroSaveOperation::None)
	{
		co_return EToroSaveLoadStatus::Waiting;
	}

	Operation = EToroSaveOperation::Saving;

	TArray<uint8> UncompressedData;
	FToroSaveGameArchive Ar(UncompressedData);
	SerializeData(Ar);
	if (Ar.IsError())
	{
		Operation = EToroSaveOperation::None;
		co_return HandleError(EToroSaveLoadStatus::SerializeFailed, SaveName);
	}

	TArray<uint8> CompressedData;
	if (!UToroOodleLibrary::OodleCompress(UncompressedData, CompressedData))
	{
		Operation = EToroSaveOperation::None;
		co_return HandleError(EToroSaveLoadStatus::CompressFailed, SaveName);
	}

	co_await UE5Coro::Async::MoveToTask();

	const bool bFileSaveResult = FFileHelper::SaveArrayToFile(CompressedData, *GetSavePath(Slot));

	co_await UE5Coro::Async::MoveToGameThread();

	Operation = EToroSaveOperation::None;
	co_return HandleError(bFileSaveResult 
		? EToroSaveLoadStatus::Succeeded 
		: EToroSaveLoadStatus::FileWriteFailed, 
		SaveName
	);
}

UE5Coro::TCoroutine<EToroSaveLoadStatus> UToroSaveGame::LoadFromFile(const uint8 Slot)
{
	if (Operation != EToroSaveOperation::None)
	{
		co_return EToroSaveLoadStatus::Waiting;
	}

	Operation = EToroSaveOperation::Loading;

	const FString SavePath = GetSavePath(Slot);
	if (!FPaths::FileExists(SavePath))
	{
		Operation = EToroSaveOperation::None;
		co_return HandleError(EToroSaveLoadStatus::FileReadFailed, SaveName);
	}

	co_await UE5Coro::Async::MoveToTask();

	TArray<uint8> CompressedData;
	const bool bFileReadResult = FFileHelper::LoadFileToArray(CompressedData, *SavePath);

	co_await UE5Coro::Async::MoveToGameThread();

	if (!bFileReadResult)
	{
		Operation = EToroSaveOperation::None;
		co_return HandleError(EToroSaveLoadStatus::FileReadFailed, SaveName);
	}

	TArray<uint8> UncompressedData;
	if (!UToroOodleLibrary::OodleDecompress(MoveTemp(CompressedData), UncompressedData))
	{
		Operation = EToroSaveOperation::None;
		co_return HandleError(EToroSaveLoadStatus::DecompressFailed, SaveName);
	}

	FToroLoadGameArchive Ar(UncompressedData);
	SerializeData(Ar);

	Operation = EToroSaveOperation::None;
	co_return HandleError(Ar.IsError() 
		? EToroSaveLoadStatus::SerializeFailed 
		: EToroSaveLoadStatus::Succeeded, 
		SaveName
	);
}

FString UToroSaveGame::GetSavePath(const uint8 Slot) const
{
	const FString Appendage = (Slot == 0) ? SaveName.ToString() 
		: FString::Printf(TEXT("Slot_%02d/%s"), Slot, *SaveName.ToString());

	return FPaths::ProjectSavedDir() / Appendage + TEXT(".sar");
}
