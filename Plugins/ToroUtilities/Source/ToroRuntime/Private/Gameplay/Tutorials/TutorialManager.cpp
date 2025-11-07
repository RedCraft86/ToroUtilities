// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Tutorials/TutorialManager.h"
#include "UserInterface/ToroWidgetManager.h"
#include "SaveSystem/ToroGlobalSave.h"
#include "ToroRuntime.h"

void UTutorialManager::QueueTutorial(const FGameplayTag Key)
{
	if (!TutorialTags::IsValidTag(Key)) return;
	const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr;
	if (!Save || !Save->Tutorials.Contains(Key))
	{
		QueueInternal(Key);
	}
}

void UTutorialManager::QueueTutorials(const TArray<FGameplayTag>& Keys)
{
	const UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr;
	for (const FGameplayTag& Key : Keys)
	{
		if (!TutorialTags::IsValidTag(Key)) continue;
		if (!Save || !Save->Tutorials.Contains(Key))
		{
			QueueInternal(Key);
		}
	}
}

bool UTutorialManager::IsShowingTutorial() const
{
	return TutorialWidget && !Tutorials.IsEmpty();
} 

void UTutorialManager::MarkTutorialsSeen()
{
	if (UToroGlobalSave* Save = SaveManager ? SaveManager->FindOrAddSave<UToroGlobalSave>() : nullptr)
	{
		Save->Tutorials.Append(Tutorials);
		Save->SaveObject(nullptr);
	}
	Tutorials.Empty();
}

const FTutorialEntry* UTutorialManager::GetTutorialData(const FGameplayTag& Key) const
{
	return Database ? Database->GetEntry(Key) : nullptr;
}

UTutorialWidget* UTutorialManager::GetTutorialWidget()
{
	if (!TutorialWidget)
	{
		TutorialWidget = AToroWidgetManager::GetWidget<UTutorialWidget>(this);
		if (TutorialWidget) TutorialWidget->SetCloseFunc([this]()
		{
			MarkTutorialsSeen();
		});
	}
	return TutorialWidget;
}

void UTutorialManager::QueueInternal(const FGameplayTag& Key)
{
	if (const FTutorialEntry* Data = Tutorials.Contains(Key) ? nullptr: GetTutorialData(Key))
	{
		Tutorials.Add(Key);
		if (UTutorialWidget* Widget = GetTutorialWidget())
		{
			Widget->ShowWidget(*Data);
		}
	}
}

void UTutorialManager::BeginPlay()
{
	Super::BeginPlay();
	SaveManager = UToroSaveManager::Get(this);
	Database = UToroSettings::Get()->GetDatabase<UTutorialDatabase>();
#if WITH_EDITOR
	if (!Database)
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Tutorial Database not found in settings!"))
	}
#endif
}
