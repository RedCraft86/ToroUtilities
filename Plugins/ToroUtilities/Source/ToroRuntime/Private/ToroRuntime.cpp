// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroRuntime.h"
#include "GeneralProjectSettings.h"
#include "NarrativeDialogueSettings.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "SaveSystem/ToroGameSave.h"

DEFINE_LOG_CATEGORY(LogToroRuntime);

#define LOCTEXT_NAMESPACE "ToroRuntime"

UToroSettings::UToroSettings()
	: MapRegistry({
		  {EToroMapType::MainMenu, nullptr},
		  {EToroMapType::Gameplay, nullptr},
		  {EToroMapType::Extras, nullptr}
	}), LightProbeLimit(16)
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroRuntime");

	LightProbePPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_LightProbe.PPM_LightProbe"));
	BrightnessPPM = FSoftObjectPath(TEXT("/ToroUtilities/Assets/PostProcess/PPM_Brightness.PPM_Brightness"));

	MainSoundMix = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SMC_Master.SMC_Master"));
	SoundClasses[0] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Master.SC_Master"));
	SoundClasses[1] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Music.SC_Music"));
	SoundClasses[2] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_SoundFX.SC_SoundFX"));
	SoundClasses[3] = FSoftObjectPath(TEXT("/Game/AssetPacks/_Globals/Audio/SC_Voice.SC_Voice"));

	InitSaves = {
		{UToroGlobalSave::StaticClass(), 0},
		{UToroGameSave::StaticClass(), 0}
	};
}

FText UToroSettings::GetVersionLabel() const
{
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		FTextBuilder Builder;
		Builder.AppendLine(INVTEXT("Version: "));
		Builder.AppendLine(ProjectSettings->ProjectVersion);
		if (!DemoName.IsNone())
		{
			Builder.AppendLineFormat(INVTEXT(" | {0}"), FText::FromName(DemoName));
		}
		return Builder.ToText();
	}
	return INVTEXT("Failed to determine version");
}

bool UToroSettings::IsOnMap(const UObject* ContextObject, const EToroMapType MapType) const
{
	const TSoftObjectPtr<UWorld>* World = MapRegistry.Find(MapType);
	return World && UGameplayStatics::GetCurrentLevelName(ContextObject) == World->GetAssetName();
}

float UToroSettings::CalcReadingTime(const FText& InText)
{
	if (InText.IsEmptyOrWhitespace())
	{
		return 0.0f;
	}

	float LettersPerSecond = 25.0f, MinDisplayTime = 2.0f;
	if (const UNarrativeDialogueSettings* DialogueSettings = GetDefault<UNarrativeDialogueSettings>())
	{
		LettersPerSecond = DialogueSettings->LettersPerSecondLineDuration;
		MinDisplayTime = DialogueSettings->MinDialogueTextDisplayTime;
	}

	return FMath::Max(InText.ToString().Len() / LettersPerSecond, MinDisplayTime);
}

#if WITH_EDITOR
void UToroSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UToroSettings, MapRegistry))
	{
		MapRegistry.FindOrAdd(EToroMapType::MainMenu);
		MapRegistry.FindOrAdd(EToroMapType::Gameplay);
		MapRegistry.FindOrAdd(EToroMapType::Extras);
	}
}
#endif

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroRuntimeModule, ToroRuntime)