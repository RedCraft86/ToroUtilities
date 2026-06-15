// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UE5Coro.h"
#include "Helpers/WorldGetter.h"
#include "CommonGameViewportClient.h"
#include "ToroGameViewportClient.generated.h"

/**
 * Custom viewport logic for handling window focus and global UI rendering.
 */
UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:

	UToroGameViewportClient();

	template<typename T = UToroGameViewportClient>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? Cast<T>(World->GetGameViewport()) : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = ScreenFade, meta = (WorldContext = ContextObject))
		static void ClearScreenFade(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = ScreenFade, meta = (WorldContext = ContextObject))
		static void SetScreenFade(const UObject* ContextObject, const FLinearColor Color, const bool bFadeSound);

	UFUNCTION(BlueprintCallable, Category = ScreenFade, meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject))
		static FVoidCoroutine StartScreenFade(FLatentActionInfo LatentInfo, const UObject* ContextObject, 
			const FLinearColor Color, const float Duration, const bool bFadeSound);

	DECLARE_MULTICAST_DELEGATE_OneParam(FFadeAudioDelegate, const float);
	FFadeAudioDelegate OnAudioFade;

	virtual void ClearScreenFade();
	virtual void SetScreenFade(const FLinearColor& Color, const bool bFadeSound);
	virtual UE5Coro::TCoroutine<> StartScreenFade(const FLinearColor& Color, const float Duration, const bool bFadeSound);

protected:

	float FadeTime;
	bool bFadeAudio;
	FLinearColor CurrentColor;
	FLinearColor TargetColor;

	void FadeAudio(const float Amount);
	virtual void Tick(float DeltaTime) override;
	virtual void PostRender(UCanvas* Canvas) override;
};
