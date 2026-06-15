// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Framework/ToroGameViewportClient.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"
#include "AudioDevice.h"

UToroGameViewportClient::UToroGameViewportClient(): FadeTime(0.0f), bFadeAudio(false)
	, CurrentColor(FLinearColor::Transparent), TargetColor(FLinearColor::Transparent)
{
}

void UToroGameViewportClient::ClearScreenFade(const UObject* ContextObject)
{
	if (UToroGameViewportClient* Viewport = Get(ContextObject))
	{
		Viewport->ClearScreenFade();
	}
}

void UToroGameViewportClient::SetScreenFade(const UObject* ContextObject, const FLinearColor Color, const bool bFadeSound)
{
	if (UToroGameViewportClient* Viewport = Get(ContextObject))
	{
		Viewport->SetScreenFade(Color, bFadeSound);
	}
}

FVoidCoroutine UToroGameViewportClient::StartScreenFade(FLatentActionInfo LatentInfo, const UObject* ContextObject,
	const FLinearColor Color, const float Duration, const bool bFadeSound)
{
	if (UToroGameViewportClient* Viewport = Get(ContextObject))
	{
		co_await Viewport->StartScreenFade(Color, Duration, bFadeSound);
	}
}

void UToroGameViewportClient::ClearScreenFade()
{
	StartScreenFade(FLinearColor::Transparent, 0.0f, false);
}

void UToroGameViewportClient::SetScreenFade(const FLinearColor& Color, const bool bFadeSound)
{
	StartScreenFade(Color, 0.0f, bFadeSound);
}

UE5Coro::TCoroutine<> UToroGameViewportClient::StartScreenFade(const FLinearColor& Color, const float Duration, const bool bFadeSound)
{
	TargetColor = Color;
	bFadeAudio = bFadeSound;
	FadeTime = FMath::Max(Duration, 0.0f);
	FadeAudio(bFadeAudio ? CurrentColor.A : 0.0f);

	co_await UE5Coro::Latent::UnpausedSeconds(FadeTime);
}

void UToroGameViewportClient::FadeAudio(const float Amount)
{
	const float Volume = FMath::Clamp(1.0f - Amount, 0.0f, 1.0f);
	if (OnAudioFade.IsBound())
	{
		OnAudioFade.Broadcast(Volume);
		return;
	}

	AudioDevice->SetTransientPrimaryVolume(Volume);
}

void UToroGameViewportClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CurrentColor.Equals(TargetColor))
	{
		if (FadeTime > 0.1f)
		{
			const float InterpSpeed = 1.0f / FadeTime;
			CurrentColor.R = FMath::FInterpConstantTo(CurrentColor.R, TargetColor.R, DeltaTime, InterpSpeed);
			CurrentColor.G = FMath::FInterpConstantTo(CurrentColor.G, TargetColor.G, DeltaTime, InterpSpeed);
			CurrentColor.B = FMath::FInterpConstantTo(CurrentColor.B, TargetColor.B, DeltaTime, InterpSpeed);
			CurrentColor.A = FMath::FInterpConstantTo(CurrentColor.A, TargetColor.A, DeltaTime, InterpSpeed);
		}
		else
		{
			CurrentColor = TargetColor;
		}

		FadeAudio(bFadeAudio ? CurrentColor.A : 0.0f);
	}
}

void UToroGameViewportClient::PostRender(UCanvas* Canvas)
{
	Super::PostRender(Canvas);
	if (CurrentColor.A > 0.01f && Canvas->DefaultTexture)
	{
		UTexture2D* Tex = Canvas->DefaultTexture;
		Canvas->DrawColor = CurrentColor.ToFColor(true);
		Canvas->DrawTile(Tex, 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY, 
			0.0f, 0.0f, Tex->GetSizeX(), Tex->GetSizeY(), BLEND_Translucent);
	}
}
