// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroOodleLibrary.generated.h"

/**
 * Blueprint-exposed version of <c>FOodleDataCompression::ECompressor</c>.
 * This enum omits the 'NotSet' (0) value from in the native enum.
 *
 * Mapping Logic: <c>Native = (UEnum + 1)</c>
 */
UENUM(BlueprintType)
enum class EOodleCompressor : uint8
{
	// Optimized for extremely fast decompression; lower compression ratio. Native: 1
	Selkie		= 0 UMETA(DisplayName = "Selkie"),

	// Fast decompression with better ratios than Selkie. Native: 2
	Mermaid		= 1 UMETA(DisplayName = "Mermaid"),

	// The standard balanced compressor; high compression with good decode speed. Native: 3
	Kraken		= 2 UMETA(DisplayName = "Kraken"),

	// Maximum compression ratio; significantly slower decompression. Native: 4
	Leviathan	= 3 UMETA(DisplayName = "Leviathan")
};
inline FOodleDataCompression::ECompressor OodleCompressorToNative(const EOodleCompressor InCompressor)
{
	return static_cast<FOodleDataCompression::ECompressor>(static_cast<uint8>(InCompressor) + 1);
}
inline EOodleCompressor NativeToOodleCompressor(const FOodleDataCompression::ECompressor InCompressor)
{
	return static_cast<EOodleCompressor>(FMath::Max(static_cast<int8>(InCompressor) - 1, 0));
}

/**
 * Blueprint-exposed version of <c>FOodleDataCompression::ECompressionLevel</c>.
 * Native values range from -4 (HyperFast) to 9 (Optimal).
 *
 * Mapping Logic: <c>Native = (UEnum - 4)</c>
 */
UENUM(BlueprintType)
enum class EOodleCompressionLevel : uint8
{
	HyperFast4	= 0,  // Native: -4
	HyperFast3	= 1,  // Native: -3
	HyperFast2	= 2,  // Native: -2
	HyperFast1	= 3,  // Native: -1
	None		= 4,  // Native:  0
	SuperFast	= 5,  // Native:  1
	VeryFast	= 6,  // Native:  2
	Fast		= 7,  // Native:  3
	Normal		= 8,  // Native:  4
	Optimal1	= 9,  // Native:  5
	Optimal2	= 10, // Native:  6
	Optimal3	= 11, // Native:  7
	Optimal4	= 12, // Native:  8
	Optimal5	= 13  // Native:  9
};
inline FOodleDataCompression::ECompressionLevel OodleCompressionLevelToNative(const EOodleCompressionLevel InLevel)
{
	return static_cast<FOodleDataCompression::ECompressionLevel>(static_cast<int8>(InLevel) - 4);
}
inline EOodleCompressionLevel NativeToOodleCompressionLevel(const FOodleDataCompression::ECompressionLevel InLevel)
{
	return static_cast<EOodleCompressionLevel>(static_cast<int8>(InLevel) + 4);
}

/**
 * A utility library providing functions for high-performance data compression via Oodle.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroOodleLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Compresses a byte array using the Oodle Data Compression library.
	 * @param InData The raw source bytes to compress.
	 * @param OutData The resulting compressed byte array.
	 * @param Compressor The Oodle algorithm to use.
	 * @param Level The compression effort level.
	 * @return True if compression succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = Compression)
		static bool OodleCompress(const TArray<uint8>& InData, TArray<uint8>& OutData,
			const EOodleCompressor Compressor = EOodleCompressor::Kraken,
			const EOodleCompressionLevel Level = EOodleCompressionLevel::SuperFast);

	/**
	 * Decompresses an Oodle-compressed byte array back to its original state.
	 * @param InData The compressed source bytes.
	 * @param OutData The resulting raw byte array.
	 * @return True if decompression succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = Compression)
		static bool OodleDecompress(const TArray<uint8>& InData, TArray<uint8>& OutData);
};
