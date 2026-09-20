// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Libraries/ToroTextureLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "Slate/WidgetRenderer.h"
#include "ImageUtils.h"
#include "ToroRuntime.h"

void FToroImageData::Empty()
{
	Size = FIntPoint::ZeroValue;
	Pixels.Empty();
}

bool FToroImageData::GetPNGData(TArray64<uint8>& OutData) const
{
	if (IsValid())
	{
		FImageUtils::PNGCompressImageArray(Size.X, Size.Y, Pixels, OutData);
		return true;
	}

	OutData.Empty();
	return false;
}

void UToroTextureLibrary::DrawWidgetToRenderTarget(UTextureRenderTarget2D* Target,
	UUserWidget* UserWidget, const bool bGammaCorrection, const bool bInClearTarget)
{
	if (IsValid(Target) && IsValid(UserWidget))
	{
		const TSharedRef<SWidget> Widget = UserWidget->TakeWidget();

		FWidgetRenderer* Renderer = new FWidgetRenderer(bGammaCorrection, bInClearTarget);
		Renderer->DrawWidget(Target, Widget,
			Widget->GetCachedGeometry().Scale,
			FVector2D(Target->SizeX, Target->SizeY),
			0.0f
		);

		BeginCleanup(Renderer);
	}
}

bool UToroTextureLibrary::GetDataFromTexture(FToroImageData& OutData, const UTexture2D* Target)
{
	OutData.Empty();
	if (!IsValid(Target))
	{
		return false;
	}

	const FTexturePlatformData* PlatformData = Target->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.IsEmpty())
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::GetDataFromTexture] Invalid PlatformData or no Mips"));
		return false;
	}

	if (PlatformData->PixelFormat != SupportedFormat)
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::GetDataFromTexture] Unsupported PixelFormat"));
		return false;
	}

	const auto& Mip = PlatformData->Mips[0];
	OutData.Size = FIntPoint(Mip.SizeX, Mip.SizeY);
	OutData.Pixels.SetNumUninitialized(OutData.Size.X * OutData.Size.Y);

	if (const void* TexData = Mip.BulkData.LockReadOnly())
	{
		FMemory::Memcpy(OutData.Pixels.GetData(), TexData, sizeof(FColor) * OutData.Pixels.Num());
		Mip.BulkData.Unlock();
		return true;
	}

	UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::GetDataFromTexture] Failed to lock BulkData"));
	OutData.Empty();
	return false;
}

bool UToroTextureLibrary::GetDataFromRenderTarget(FToroImageData& OutData, UTextureRenderTarget2D* Target, const bool bInvertAlpha)
{
	OutData.Empty();
	if (!IsValid(Target))
	{
		return false;
	}

	OutData.Size = FIntPoint(Target->SizeX, Target->SizeY);
	if (FTextureRenderTargetResource* Resource = Target->GameThread_GetRenderTargetResource())
	{
		if (!Resource->ReadPixels(OutData.Pixels))
		{
			OutData.Empty();
			return false;
		}

		if (bInvertAlpha)
		{
			for (FColor& Pixel : OutData.Pixels)
			{
				Pixel.A = 255 - Pixel.A;
			}
		}

		return true;
	}

	return false;
}

UTexture2D* UToroTextureLibrary::CreateTextureFromData(const FToroImageData& InData)
{
	if (!InData.IsValid())
	{
		return nullptr;
	}

	UTexture2D* Image = UTexture2D::CreateTransient(InData.Size.X, InData.Size.Y, SupportedFormat);
	if (!IsValid(Image))
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::CreateTextureFromData] Failed to create texture"));
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
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::CreateTextureFromData] Invalid PlatformData or Mips"));
		Image->MarkAsGarbage();
		return nullptr;
	}

	FTexture2DMipMap& Mip = PlatformData->Mips[0];
	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
	if (!TextureData)
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::CreateTextureFromData] Failed to lock BulkData"));
		Image->MarkAsGarbage();
		return nullptr;
	}

	FMemory::Memcpy(TextureData, InData.Pixels.GetData(), sizeof(FColor) * InData.Size.X * InData.Size.Y);
	Mip.BulkData.Unlock();

	Image->UpdateResource();

	return Image;
}

UTexture2D* UToroTextureLibrary::ConvertRenderTargetToTexture(UTextureRenderTarget2D* Target, const bool bInvertAlpha)
{
	FToroImageData Data;
	if (GetDataFromRenderTarget(Data, Target, bInvertAlpha))
	{
		return CreateTextureFromData(Data);
	}

	return nullptr;
}

FVoidCoroutine UToroTextureLibrary::SaveTextureToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
	const UTexture2D* Target, const FString& FilePath, const bool bAsync)
{
	FToroImageData Data;
	if (!GetDataFromTexture(Data, Target))
	{
		bSuccess = false;
		co_return;
	}

	co_await SaveImageDataToFile(FLatentActionInfo(), bSuccess, Data, FilePath, bAsync);
	co_return;
}

FVoidCoroutine UToroTextureLibrary::SaveRenderTargetToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
	UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha, const bool bAsync)
{
	FToroImageData Data;
	if (!GetDataFromRenderTarget(Data, Target, bInvertAlpha))
	{
		bSuccess = false;
		co_return;
	}

	co_await SaveImageDataToFile(FLatentActionInfo(), bSuccess, Data, FilePath, bAsync);
	co_return;
}

FVoidCoroutine UToroTextureLibrary::SaveImageDataToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
	const FToroImageData& InData, const FString& FilePath, const bool bAsync)
{
	FString SavePath = FilePath;
	if (FPaths::GetExtension(FilePath) != TEXT("png"))
	{
		SavePath.Append(TEXT(".png"));
	}

	if (!FPaths::ValidatePath(SavePath))
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::SaveImageDataToFile] Export failed due to invalid path"));
		bSuccess = false;
		co_return;
	}

	TArray64<uint8> PNGImage;
	if (!InData.GetPNGData(PNGImage))
	{
		UE_LOG(LogToroRuntime, Warning, TEXT("[UToroTextureLibrary::SaveImageDataToFile] Export failed due to invalid data"));
		bSuccess = false;
		co_return;
	}

	if (bAsync)
	{
		co_await UE5Coro::Async::MoveToTask();
		const bool bResult = FFileHelper::SaveArrayToFile(PNGImage, *SavePath);
		co_await UE5Coro::Async::MoveToGameThread();
		bSuccess = bResult;
		co_return;
	}

	bSuccess = FFileHelper::SaveArrayToFile(PNGImage, *SavePath);
}
