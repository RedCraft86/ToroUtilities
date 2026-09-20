// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "UE5Coro.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroTextureLibrary.generated.h"

/**
 * Stores image dimensions and 8-bit color pixels for texture conversion and PNG export.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FToroImageData
{
	GENERATED_BODY()

	/**
	 * Width and height of the image in pixels.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImageData)
	FIntPoint Size;

	/**
	 * Row-major image pixels; the count must equal width times height.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImageData)
	TArray<FColor> Pixels;

	FToroImageData()
		: Size(FIntPoint::ZeroValue), Pixels({})
	{}

	FToroImageData(const FIntPoint& Size, const TArray<FColor>& InPixels)
		: Size(Size), Pixels(InPixels)
	{}

	FToroImageData(const int32 SizeX, const int32 SizeY, const TArray<FColor>& InPixels)
		: Size(SizeX, SizeY), Pixels(InPixels)
	{}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FToroImageData& ImageData)
	{
		Ar << ImageData.Size;
		Ar << ImageData.Pixels;
		return Ar;
	}

	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, FToroImageData& ImageData)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << SA_VALUE(TEXT("Size"), ImageData.Size) << SA_VALUE(TEXT("Pixels"), ImageData.Pixels);
	}

	/**
	 * Clears the pixels and resets the dimensions to zero.
	 */
	void Empty();

	/**
	 * Compresses valid image pixels into PNG bytes.
	 * @param OutData Receives the encoded PNG, or is cleared when the image is invalid.
	 * @return Whether the source image data is valid.
	 */
	bool GetPNGData(TArray64<uint8>& OutData) const;

	/**
	 * Checks that the dimensions are positive and match the pixel count.
	 */
	FORCEINLINE bool IsValid() const
	{
		return Size.X > 0 && Size.Y > 0 && static_cast<int64>(Pixels.Num()) == static_cast<int64>(Size.X) * static_cast<int64>(Size.Y);
	}
};

/**
 * Utility functions for image processing.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroTextureLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static constexpr EPixelFormat SupportedFormat = PF_B8G8R8A8;

	/**
	 * Renders a widget into a texture render target.
	 * @param Target Destination render target.
	 * @param UserWidget Widget to render.
	 * @param bGammaCorrection Whether to apply gamma correction.
	 * @param bInClearTarget Whether to clear the target before drawing.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static void DrawWidgetToRenderTarget(UTextureRenderTarget2D* Target, UUserWidget* UserWidget,
			const bool bGammaCorrection = true, const bool bInClearTarget = false);

	/**
	 * Reads the first mip of a BGRA8 texture into image data.
	 * @param OutData Receives pixel data, or is reset on failure.
	 * @param Target Texture to read.
	 * @return Whether the texture pixels were read.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool GetDataFromTexture(FToroImageData& OutData, const UTexture2D* Target);

	/**
	 * Reads render target pixels into image data.
	 * @param OutData Receives pixel data, or is reset when the target is unavailable.
	 * @param Target Render target to read.
	 * @param bInvertAlpha Whether to invert each pixel alpha.
	 * @return Whether a render target resource was available.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool GetDataFromRenderTarget(FToroImageData& OutData, UTextureRenderTarget2D* Target, const bool bInvertAlpha);

	/**
	 * Creates a transient BGRA8 texture from image data.
	 * @param InData Source dimensions and pixels.
	 * @return The new texture, or nullptr when the data or allocation is invalid.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* CreateTextureFromData(const FToroImageData& InData);

	/**
	 * Copies render target pixels into a transient texture.
	 * @param Target Render target to convert.
	 * @param bInvertAlpha Whether to invert each pixel alpha.
	 * @return The new texture, or nullptr if the conversion fails.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* ConvertRenderTargetToTexture(UTextureRenderTarget2D* Target, const bool bInvertAlpha);

	/**
	 * Saves a BGRA8 texture as a PNG file.
	 * @param bSuccess Receives whether the file was saved.
	 * @param Target Texture to save.
	 * @param FilePath Output path; .png is appended if absent.
	 * @param bAsync Whether to write the file on a task thread.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine SaveTextureToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
			const UTexture2D* Target, const FString& FilePath, const bool bAsync = true);

	/**
	 * Saves a render target as a PNG file.
	 * @param bSuccess Receives whether the file was saved.
	 * @param Target Render target to save.
	 * @param FilePath Output path; .png is appended if absent.
	 * @param bInvertAlpha Whether to invert each pixel alpha.
	 * @param bAsync Whether to write the file on a task thread.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine SaveRenderTargetToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
			UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha, const bool bAsync = true);

	/**
	 * Encodes image data as PNG and saves it to a file.
	 * @param bSuccess Receives whether the file was saved.
	 * @param InData Source image data.
	 * @param FilePath Output path; .png is appended if absent.
	 * @param bAsync Whether to write the file on a task thread.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine SaveImageDataToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
			const FToroImageData& InData, const FString& FilePath, const bool bAsync = true);
};
