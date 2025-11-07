// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Narrative/NarrativeManager.h"
#include "UserInterface/ToroManagedWidget.h"
#include "SubtitleWidget.generated.h"

USTRUCT(BlueprintType)
struct FToroSubtitle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subtitle)
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subtitle)
		FText Line;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Subtitle, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float ExtraTime;

	FToroSubtitle(): ExtraTime(0.0f) {}
	FToroSubtitle(const FText& InName, const FText& InLine, const float InExtraTime = 0.0f)
		: Name(InName), Line(InLine), ExtraTime(InExtraTime)
	{}

	bool IsPlayer() const { return Name.EqualTo(INVTEXT("You")); }
	float CalcTime() const { return ExtraTime + UToroSettings::Get()->CalcReadingTime(Line); }
	bool IsValidData() const { return !Line.IsEmptyOrWhitespace() && ExtraTime >= 0.0f; }
	bool Matches(const FToroSubtitle& Other) const
	{
		return Name.EqualTo(Other.Name) && Line.EqualTo(Other.Line);
	}
};

UCLASS(Abstract)
class TORORUNTIME_API USubtitleWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	USubtitleWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle);

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> LineText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SubtitleAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		FLinearColor NameColor;

	UPROPERTY(EditAnywhere, Category = Settings)
		FLinearColor PlayerColor;

	FTimerHandle SubtitleTimer;
	TArray<FToroSubtitle> Subtitles;

	void NextSubtitle();
	void ShowSubtitle(const FToroSubtitle& InData);
	void AddSubtitle(const FToroSubtitle& InSubtitle);
	void AddSubtitles(const TArray<FToroSubtitle>& InSubtitles);
	void OnNativeSubtitle(const FText& InText);

	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
