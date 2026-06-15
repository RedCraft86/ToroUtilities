// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UE5Coro.h"
#include "ImageUtils.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroTextureLibrary.generated.h"

/**
 * A light-weight container for raw pixel data (FColor) and image dimensions.
 * This structure is primarily used to bridge the gap between GPU-side textures 
 * and CPU-side data processing/serialization.
 * 
 * Includes built-in support for PNG compression and Archive serialization.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FGenericImageData
{
	GENERATED_BODY()

	/** The width and height of the image in pixels. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImageData)
		FIntPoint Size;

	/** 
	 * The raw pixel buffer. 
	 * Note: For this struct to be valid, Pixels.Num() must equal Size.X * Size.Y.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImageData)
		TArray<FColor> Pixels;

	FGenericImageData(): Size(FIntPoint::ZeroValue), Pixels({}) {}
	FGenericImageData(const FIntPoint& Size, const TArray<FColor>& InPixels): Size(Size), Pixels(InPixels) {}
	FGenericImageData(const int32 SizeX, const int32 SizeY, const TArray<FColor>& InPixels)
		: Size(SizeX, SizeY), Pixels(InPixels)
	{}

	bool GetData(TArray64<uint8>& OutData) const
	{
		if (IsValid())
		{
			FImageUtils::PNGCompressImageArray(Size.X, Size.Y, Pixels, OutData);
			return true;
		}
		OutData.Empty();
		return false;
	}

	void Reset()
	{
		Size = FIntPoint::ZeroValue;
		Pixels.Empty();
	}

	FORCEINLINE [[nodiscard]] bool IsValid() const { return !Pixels.IsEmpty() && Pixels.Num() == Size.X * Size.Y; }

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGenericImageData& ImageData)
	{
		Ar << ImageData.Pixels;
		Ar << ImageData.Size;
		return Ar;
	}
};

/**
 * A Blueprint Function Library providing advanced texture manipulation utilities.
 * 
 * Features:
 * - Dynamic extraction of pixel data from Texture2D and RenderTargets.
 * - Runtime creation of Texture2D assets from raw pixel data.
 * - Synchronous and Asynchronous (Latent) image saving to disk.
 * - Widget-to-RenderTarget rendering wrappers.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroTextureLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Renders a UUserWidget directly into a Render Target. */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static void DrawWidgetToRenderTarget(UTextureRenderTarget2D* Target, class UUserWidget* UserWidget);

	/** 
	 * Extracts pixel data from a Render Target into a GenericImageData struct. 
	 * @param OutData     The struct to receive the pixel data (Output).
	 * @param Target      The Render Target to read from.
	 * @param bInvertAlpha If true, the alpha channel will be inverted (useful for certain export formats).
	 * @return True if pixels were successfully read.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool GetDataFromRenderTarget(FGenericImageData& OutData, UTextureRenderTarget2D* Target, const bool bInvertAlpha);

	/** Extracts pixel data from a standard Texture2D. Note: Texture must be CPU-accessible (e.g., UI or set to 'AllowCPURead'). */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool GetDataFromTexture(FGenericImageData& OutData, const UTexture2D* Target);

	/** Creates a new transient Texture2D at runtime from the provided pixel data. */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* CreateTextureFromData(const FGenericImageData& InData);

	/** Converts a Render Target's current contents into a permanent Texture2D asset. */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static UTexture2D* ConvertRenderTargetToTexture(UTextureRenderTarget2D* Target, const bool bHasAlpha);

	/** Saves the provided image data to a file (PNG format) on disk synchronously. */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool SaveTextureDataToFile(const FGenericImageData& InData, const FString& FilePath);

	/** Saves a Texture2D asset to a file (PNG format) on disk synchronously. */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool SaveTextureToFile(const UTexture2D* Target, const FString& FilePath);

	/** Saves a Render Target to a file (PNG format) on disk synchronously. */
	UFUNCTION(BlueprintCallable, Category = TextureUtils)
		static bool SaveRenderTargetToFile(UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha = false);

	/** 
	 * Asynchronously saves the provided image data to a file (PNG format) on disk. 
	 * This node will block execution in the Blueprint graph until the file write is complete.
	 * 
	 * @param bSuccess   Returns true if the file was saved successfully (Output).
	 * @param InData     The image data to save.
	 * @param FilePath   Full disk path (including extension) where the file should be saved.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine AsyncSaveTextureDataToFile(FLatentActionInfo LatentInfo, bool& bSuccess, const FGenericImageData& InData, const FString& FilePath);

	/** 
	 * Asynchronously saves a Texture2D asset to a file (PNG format) on disk. 
	 * This node will block execution in the Blueprint graph until the file write is complete.
	 * 
	 * @param bSuccess   Returns true if the file was saved successfully (Output).
	 * @param Target     The texture to save.
	 * @param FilePath   Full disk path (including extension) where the file should be saved.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine AsyncSaveTextureToFile(FLatentActionInfo LatentInfo, bool& bSuccess, const UTexture2D* Target, const FString& FilePath);

	/** 
	 * Asynchronously saves a Render Target to a file (PNG format) on disk. 
	 * This node will block execution in the Blueprint graph until the file write is complete.
	 * 
	 * @param bSuccess   Returns true if the file was saved successfully (Output).
	 * @param Target     The render target to save.
	 * @param FilePath   Full disk path (including extension) where the file should be saved.
	 */
	UFUNCTION(BlueprintCallable, Category = TextureUtils, meta = (Latent, LatentInfo = LatentInfo))
		static FVoidCoroutine AsyncSaveRenderTargetToFile(FLatentActionInfo LatentInfo, bool& bSuccess, UTextureRenderTarget2D* Target, const FString& FilePath, const bool bInvertAlpha);

private:

	/** Internal helper to execute the file export logic. */
	static bool ExportImageData(const FGenericImageData& InData, const FString& FilePath);
};
