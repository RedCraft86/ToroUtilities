// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Libraries/ToroOodleLibrary.h"
#include "Compression/OodleDataCompressionUtil.h"

bool UToroOodleLibrary::OodleCompress(const TArray<uint8>& InData, TArray<uint8>& OutData,
	const EOodleCompressor Compressor, const EOodleCompressionLevel Level)
{
	OutData.Empty();
	return !InData.IsEmpty() && FOodleCompressedArray::CompressTArray(OutData, InData, 
		OodleCompressorToNative(Compressor), OodleCompressionLevelToNative(Level));
}

bool UToroOodleLibrary::OodleDecompress(const TArray<uint8>& InData, TArray<uint8>& OutData)
{
	OutData.Empty();
	return !InData.IsEmpty() && FOodleCompressedArray::DecompressToTArray(OutData, InData);
}
