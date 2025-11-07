// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/Elements/ToroButtonList.h"
#include "Components/VerticalBoxSlot.h"
#include "Helpers/WidgetAnimHelpers.h"
#include "Blueprint/WidgetTree.h"

void UToroButtonListEntry::SelectButton(bool bImmediate)
{
	if (!bSelected)
	{
		bSelected = true;
		SyncSelectionState(bImmediate);
	}
}

void UToroButtonListEntry::DeselectButton(bool bImmediate)
{
	if (bSelected)
	{
		bSelected = false;
		SyncSelectionState(bImmediate);
	}
}

void UToroButtonListEntry::SetDisplayData(const FText& InText, const FSlateFontInfo& InFont) const
{
	Label->SetText(InText);
	Label->SetFont(InFont);
}

void UToroButtonListEntry::OnButtonClicked()
{
	if (OnClicked.IsBound())
	{
		SelectButton();
		OnClicked.Execute(this);
	}
}

void UToroButtonListEntry::SyncSelectionState(const bool bImmediate)
{
	WidgetAnimHelpers::PlayOrSnapAnim(this, SelectAnim, bSelected, bImmediate);
}

void UToroButtonListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddUniqueDynamic(this, &UToroButtonListEntry::OnButtonClicked);
}

UToroButtonList::UToroButtonList(const FObjectInitializer& ObjectInitializer)
	: UVerticalBox(ObjectInitializer), EntryPadding(0.0f, 2.0f), SelectedIndex(INDEX_NONE)
{
#if WITH_EDITOR
	ConstructorHelpers::FClassFinder<UToroButtonListEntry> EntryFinder(
		TEXT("/ToroUtilities/Widgets/DefaultButtonListEntry.DefaultButtonListEntry_C"));
	if (EntryFinder.Succeeded())
	{
		EntryClass = EntryFinder.Class;
	}

	const ConstructorHelpers::FObjectFinder<UObject> FontFinder(TEXT("/Engine/EngineFonts/Roboto.Roboto"));
	if (FontFinder.Succeeded())
	{
		EntryFont = FSlateFontInfo(FontFinder.Object, 29, "Regular");
	}
#endif
}

void UToroButtonList::OnSelectionMade(UToroButtonListEntry* Button)
{
	for (int i = 0; i < Buttons.Num(); i++)
	{
		if (Buttons[i] == Button)
		{
			SelectedIndex = i;
			OnSelection.Broadcast(i);
			OnSelectionBP.Broadcast(i);
		}
		else
		{
			Buttons[i]->DeselectButton();
		}
	}
}

void UToroButtonList::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	ClearChildren();
	SelectedIndex = INDEX_NONE;
	Buttons.Empty(Entries.Num());
	if (!EntryClass) return;

	for (const FText& Entry : Entries)
	{
		if (Entry.IsEmptyOrWhitespace()) continue;
		if (UToroButtonListEntry* Button = CreateWidget<UToroButtonListEntry>(this, EntryClass))
		{
			Buttons.Add(Button);
			Button->SetDisplayData(Entry, EntryFont);
			if (UVerticalBoxSlot* EntrySlot = AddChildToVerticalBox(Button))
			{
				EntrySlot->SetSize(EntrySize);
				EntrySlot->SetPadding(EntryPadding);
			}
#if WITH_EDITOR
			if (IsDesignTime())
				continue;
#endif
			Button->OnClicked.BindUObject(this, &UToroButtonList::OnSelectionMade);
			if (Buttons.Num() == 1)
			{
				SelectedIndex = 0;
				Button->SelectButton(true); // Select first
			}
		}
	}
}

void UToroButtonList::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	for (UToroButtonListEntry* Button : Buttons)
	{
		if (Button) Button->OnClicked.Unbind();
	}
}