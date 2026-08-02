// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "UE5Coro.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroTextureLibrary.generated.h"

/**
 * A generic container for raw pixel data (FColor) and image dimensions.
 *
 * Note: For this struct to be valid, <c>Pixels.Num()</c> must equal <c>Size.X * Size.Y</c>
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FToroImageData
{
	GENERATED_BODY()

	/**
	 * The width and height of the image in pixels.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImageData)
		FIntPoint Size;

	/**
	 * The raw pixel buffer.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImageData)
		TArray<FColor> Pixels;

	FToroImageData(): Size(FIntPoint::ZeroValue), Pixels({}) {}
	FToroImageData(const FIntPoint& Size, const TArray<FColor>& InPixels): Size(Size), Pixels(InPixels) {}
	FToroImageData(const int32 SizeX, const int32 SizeY, const TArray<FColor>& InPixels)
		: Size(SizeX, SizeY), Pixels(InPixels)
	{}

	void Reset();
	bool GetData(TArray64<uint8>& OutData) const;

	FORCEINLINE [[nodiscard]] bool IsValid() const
	{
		return !Pixels.IsEmpty() && Pixels.Num() == Size.X * Size.Y;
	}

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
};

/**
 * A utility library providing functions for runtime texture management.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroTextureLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Renders a <c>UUserWidget</c> directly into a <c>UTextureRenderTarget2D</c>.
	 * @param Target Render target to render the widget onto.
	 * @param UserWidget Widget to render on the render target.
	 * @param bGammaCorrection If true, assumes render target is Linear color space and applies a gamma curve to slate.
	 * @param bInClearTarget If true, clears existing data on the render target before rendering the widget onto it.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static void DrawWidgetToRenderTarget(UTextureRenderTarget2D* Target, UUserWidget* UserWidget,
			const bool bGammaCorrection = true, const bool bInClearTarget = false);

	/**
	 * Extracts pixel and size data from the provided <c>UTexture2D</c>.
	 * @param Target Texture to extract the data from.
	 * @param OutData Data extracted from the texture.
	 * @return True if data was successfully extracted.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool GetDataFromTexture(FToroImageData& OutData, const UTexture2D* Target);

	/**
	 * Extracts pixel and size data from the provided <c>UTextureRenderTarget2D</c>.
	 * @param Target Render target to extract the data from.
	 * @param bInvertAlpha If the alpha channel of the render target should be inverted.
	 * @param OutData Data extracted from the render target.
	 * @return True if data was successfully extracted.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool GetDataFromRenderTarget(FToroImageData& OutData, UTextureRenderTarget2D* Target, const bool bInvertAlpha);

	/**
	 * Creates a new <c>UTexture2D</c> from the provided <c>FToroImageData</c> struct.
	 * @param InData Data to generate the Texture2D from.
	 * @return Newly created texture generated from the image data if conversion was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* CreateTextureFromData(const FToroImageData& InData);

	/**
	 * Converts <c>UTextureRenderTarget2D</c> to <c>UTexture2D</c> by taking the image data and building a new Tetxure.
	 * @param Target TextureRenderTarget2D to be converted to a Texture2D.
	 * @param bInvertAlpha If the alpha channel of the render target should be inverted.
	 * @return Newly created texture with the same data from the render target if conversion was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* ConvertRenderTargetToTexture(UTextureRenderTarget2D* Target, const bool bInvertAlpha);

	/**
	 * Exports a <c>UTexture2D</c> as a physical <c>.png</c> file on disk.
	 * @param Target Texture2D to be exported to file.
	 * @param FilePath Path and filename to save the image to. Extension should be excluded or use <c>.png</c>.
	 * @param bAsync Whether the file write process should be asynchronous (recommended to prevent stutters).
	 * @param bSuccess True if the export was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine SaveTextureToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
			const UTexture2D* Target, const FString& FilePath, const bool bAsync = true);

	/**
	 * Exports a <c>UTextureRenderTarget2D</c> as a physical <c>.png</c> file on disk.
	 * @param Target TextureRenderTarget2D to be exported to file.
	 * @param FilePath Path and filename to save the image to. Extension should be excluded or use <c>.png</c>.
	 * @param bInvertAlpha If the alpha channel of the render target should be inverted.
	 * @param bAsync Whether the file write process should be asynchronous (recommended to prevent stutters).
	 * @param bSuccess True if the export was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine SaveRenderTargetToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
			UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha, const bool bAsync = true);

	/**
	 * Exports a <c>FToroImageData</c> as a physical <c>.png</c> file on disk.
	 * @param InData Image Data to be exported to file.
	 * @param FilePath Path and filename to save the image to. Extension should be excluded or use <c>.png</c>.
	 * @param bAsync Whether the file write process should be asynchronous (recommended to prevent stutters).
	 * @param bSuccess True if the export was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine SaveImageDataToFile(FLatentActionInfo LatentInfo, bool& bSuccess,
			const FToroImageData& InData, const FString& FilePath, const bool bAsync = true);

private:

	/**
	 * The supported texture format that conversion functions use.
	 */
	static constexpr EPixelFormat SupportedFormat = PF_B8G8R8A8;
};
