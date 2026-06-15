// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Libraries/ToroTextureLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "Slate/WidgetRenderer.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include "TextureResource.h"
#include "ToroCore.h"

void UToroTextureLibrary::DrawWidgetToRenderTarget(UTextureRenderTarget2D* Target, class UUserWidget* UserWidget)
{
	if (Target && UserWidget)
	{
		const TSharedRef<SWidget> Widget = UserWidget->TakeWidget();
		FWidgetRenderer* Renderer = new FWidgetRenderer(true, false);
		Renderer->DrawWidget(Target, Widget, Widget->GetCachedGeometry().Scale,
			FVector2D(Target->SizeX, Target->SizeY), 0.0f);
		BeginCleanup(Renderer);
	}
}

bool UToroTextureLibrary::GetDataFromRenderTarget(FGenericImageData& OutData, UTextureRenderTarget2D* Target, const bool bInvertAlpha)
{
	if (!Target)
	{
		return false;
	}

	OutData.Size = FIntPoint(Target->SizeX, Target->SizeY);
	Target->GameThread_GetRenderTargetResource()->ReadPixels(OutData.Pixels);
	if (bInvertAlpha)
	{
		for (FColor& Pixel : OutData.Pixels)
		{
			Pixel.A = 255 - Pixel.A;
		}
	}
	return true;
}

bool UToroTextureLibrary::GetDataFromTexture(FGenericImageData& OutData, const UTexture2D* Target)
{
	OutData.Reset();
	if (!Target) return false;

	const FTexturePlatformData* PlatformData = Target->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		UE_LOG(LogToroCore, Warning, TEXT("[ToroTextureUtils:%d] Invalid PlatformData or no Mips"), __LINE__);
		return false;
	}

	if (PlatformData->PixelFormat != PF_B8G8R8A8)
	{
		UE_LOG(LogToroCore, Warning, TEXT("[ToroTextureUtils:%d] Unsupported PixelFormat %d"), __LINE__, PlatformData->PixelFormat);
		return false;
	}

	const auto& Mip = PlatformData->Mips[0];
	OutData.Size = FIntPoint(Mip.SizeX, Mip.SizeY);
	OutData.Pixels.SetNumUninitialized(OutData.Size.X * OutData.Size.Y);

	if (const void* TexData = Mip.BulkData.LockReadOnly())
	{
		FMemory::Memcpy(OutData.Pixels.GetData(), TexData, sizeof(FColor) * OutData.Pixels.Num());
		Mip.BulkData.Unlock();
	}
	else
	{
		UE_LOG(LogToroCore, Warning, TEXT("[ToroTextureUtils:%d] Failed to lock BulkData"), __LINE__);
		OutData.Reset();
	}

	return true;
}

UTexture2D* UToroTextureLibrary::CreateTextureFromData(const FGenericImageData& InData)
{
	if (!InData.IsValid())
	{
		return nullptr;
	}

	UTexture2D* Image = UTexture2D::CreateTransient(InData.Size.X, InData.Size.Y, PF_B8G8R8A8);
	if (!Image)
	{
		UE_LOG(LogToroCore, Warning, TEXT("[ToroTextureUtils:%d] Failed to create texture"), __LINE__);
		return nullptr;
	}

#if WITH_EDITORONLY_DATA
	Image->MipGenSettings = TMGS_NoMipmaps;
#endif
	Image->CompressionSettings = TC_Default;
	Image->SRGB = true;

	FTexturePlatformData* PlatformData = Image->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		UE_LOG(LogToroCore, Warning, TEXT("[ToroTextureUtils:%d] Invalid PlatformData or Mips"), __LINE__);
		return nullptr;
	}

	FTexture2DMipMap& Mip = PlatformData->Mips[0];
	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
	if (!TextureData)
	{
		UE_LOG(LogToroCore, Warning, TEXT("[ToroTextureUtils:%d] Failed to lock BulkData"), __LINE__);
		return nullptr;
	}

	FMemory::Memcpy(TextureData, InData.Pixels.GetData(), sizeof(FColor) * InData.Size.X * InData.Size.Y);
	Mip.BulkData.Unlock();

	Image->UpdateResource();

	return Image;
}

UTexture2D* UToroTextureLibrary::ConvertRenderTargetToTexture(UTextureRenderTarget2D* Target, const bool bHasAlpha)
{
	FGenericImageData Data;
	if (GetDataFromRenderTarget(Data, Target, bHasAlpha))
	{
		return CreateTextureFromData(Data);
	}
	return nullptr;
}

bool UToroTextureLibrary::SaveTextureDataToFile(const FGenericImageData& InData, const FString& FilePath)
{
	FString SavePath = FilePath;
	if (FPaths::GetExtension(FilePath) != TEXT("png"))
	{
		SavePath.Append(TEXT(".png"));
	}
	
	if (InData.IsValid() && FPaths::ValidatePath(SavePath))
	{
		return ExportImageData(InData, SavePath);
	}

	return false;
}

bool UToroTextureLibrary::SaveTextureToFile(const UTexture2D* Target, const FString& FilePath)
{
	FGenericImageData Data;
	return GetDataFromTexture(Data, Target) && SaveTextureDataToFile(Data, FilePath);
}

bool UToroTextureLibrary::SaveRenderTargetToFile(UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha)
{
	FGenericImageData Data;
	return GetDataFromRenderTarget(Data, Target, bInvertAlpha) && SaveTextureDataToFile(Data, FilePath);
}

FVoidCoroutine UToroTextureLibrary::AsyncSaveTextureDataToFile(FLatentActionInfo LatentInfo, bool& bSuccess, const FGenericImageData& InData, const FString& FilePath)
{
	co_await UE5Coro::Async::MoveToTask();
	const bool bResult = SaveTextureDataToFile(InData, FilePath);
	co_await UE5Coro::Async::MoveToGameThread();
	bSuccess = bResult;
}

FVoidCoroutine UToroTextureLibrary::AsyncSaveTextureToFile(FLatentActionInfo LatentInfo, bool& bSuccess, const UTexture2D* Target, const FString& FilePath)
{
	co_await UE5Coro::Async::MoveToTask();
	const bool bResult = SaveTextureToFile(Target, FilePath);
	co_await UE5Coro::Async::MoveToGameThread();
	bSuccess = bResult;
}

FVoidCoroutine UToroTextureLibrary::AsyncSaveRenderTargetToFile(FLatentActionInfo LatentInfo, bool& bSuccess, UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha)
{
	co_await UE5Coro::Async::MoveToTask();
	const bool bResult = SaveRenderTargetToFile(Target, FilePath, bInvertAlpha);
	co_await UE5Coro::Async::MoveToGameThread();
	bSuccess = bResult;
}

bool UToroTextureLibrary::ExportImageData(const FGenericImageData& InData, const FString& FilePath)
{
	TArray64<uint8> Image;
	if (InData.GetData(Image))
	{
		return FFileHelper::SaveArrayToFile(Image, *FilePath);
	}

	UE_LOG(LogToroCore, Warning, TEXT("UToroTextureLibrary::ExportImageData failed due to invalid data"));
	return false;
}
