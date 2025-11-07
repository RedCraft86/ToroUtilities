// Copyright (C) RedCraft86. All Rights Reserved.

#include "Libraries/ToroTextureUtils.h"

#include "ImageUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "Slate/WidgetRenderer.h"

void UToroTextureUtils::DrawWidgetToRenderTarget(UTextureRenderTarget2D* InRenderTarget, UUserWidget* InUserWidget)
{
	if (IsValid(InRenderTarget) && IsValid(InUserWidget))
	{
		const TSharedRef<SWidget> Widget = InUserWidget->TakeWidget();
		FWidgetRenderer* Renderer = new FWidgetRenderer(true, false);
		Renderer->DrawWidget(InRenderTarget, Widget, Widget->GetCachedGeometry().Scale,
			FVector2D(InRenderTarget->SizeX, InRenderTarget->SizeY), 0.0f);
	}
}

FToroTextureData UToroTextureUtils::GetDataFromRenderTarget(UTextureRenderTarget2D* InRenderTarget, const bool bInvertAlpha)
{
	FToroTextureData Result;
	if (IsValid(InRenderTarget))
	{
		InRenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(Result.Pixels);
		Result.Size = FIntPoint(InRenderTarget->SizeX, InRenderTarget->SizeY);
		if (bInvertAlpha)
		{
			for (FColor& Pixel : Result.Pixels)
			{
				Pixel.A = (uint8)((int32)Pixel.A * -1 + 255);
			}
		}
	}
	return Result;
}

FToroTextureData UToroTextureUtils::GetDataFromTexture(const UTexture2D* InTexture)
{
	FToroTextureData Result;
	if (IsValid(InTexture))
	{
		const FTexturePlatformData* PlatformData = InTexture->GetPlatformData();
		Result.Size = FIntPoint(PlatformData->SizeX, PlatformData->SizeY);
		Result.Pixels.AddDefaulted(Result.Size.X * Result.Size.Y);
		const void* TexData = PlatformData->Mips[0].BulkData.LockReadOnly();
		FMemory::Memcpy(Result.Pixels.GetData(), TexData, sizeof(FColor) * Result.Pixels.Num());
		PlatformData->Mips[0].BulkData.Unlock();
	}
	return Result;
}

UTexture2D* UToroTextureUtils::CreateTextureFromData(const FToroTextureData& InData)
{
	UTexture2D* Image = nullptr;
	if (InData.IsValidData())
	{
		Image = UTexture2D::CreateTransient(InData.Size.X, InData.Size.Y, PF_B8G8R8A8);
		FTexturePlatformData* PlatformData = Image->GetPlatformData();
		void* TextureData = PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(TextureData, InData.Pixels.GetData(), sizeof(FColor) * InData.Size.X * InData.Size.Y);
		PlatformData->Mips[0].BulkData.Unlock();
		Image->UpdateResource();
	}
	return Image;
}

UTexture2D* UToroTextureUtils::ConvertRenderTargetToTexture(UTextureRenderTarget2D* InRenderTarget, const bool bHasAlpha)
{
	return CreateTextureFromData(GetDataFromRenderTarget(InRenderTarget, bHasAlpha));
}

void UToroTextureUtils::SaveTextureDataToFile(const FToroTextureData& InData,
	const FString& InPath, const bool bAsync, const FString& FileExtension)
{
	if (FPaths::ValidatePath(InPath) && InData.IsValidData())
	{
		AsyncTask(bAsync ? ENamedThreads::AnyBackgroundHiPriTask : ENamedThreads::GameThread, [InData, InPath, FileExtension]() {
			TArray64<uint8> ImageBinary;
			FImageUtils::PNGCompressImageArray(InData.Size.X, InData.Size.Y, InData.Pixels, ImageBinary);
			FFileHelper::SaveArrayToFile(ImageBinary, *(InPath + (FileExtension.IsEmpty() ? TEXT(".png") : FileExtension)));
		});
	}
}

void UToroTextureUtils::SaveTextureToFile(const UTexture2D* InTexture,
	const FString& InPath, const bool bAsync, const FString& FileExtension)
{
	if (FPaths::ValidatePath(InPath) && IsValid(InTexture))
	{
		SaveTextureDataToFile(GetDataFromTexture(InTexture), InPath, bAsync, FileExtension);
	}
}

void UToroTextureUtils::SaveRenderTargetToFile(UTextureRenderTarget2D* InRenderTarget,
	const FString& InPath, const bool bHasAlpha, const bool bAsync, const FString& FileExtension)
{
	if (FPaths::ValidatePath(InPath) && IsValid(InRenderTarget))
	{
		SaveTextureDataToFile(GetDataFromRenderTarget(InRenderTarget, bHasAlpha), InPath, bAsync, FileExtension);
	}
}
