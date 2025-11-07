// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "Components/Overlay.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ToroContainerWidget.generated.h"

class UToroManagedWidget;

/**
 * A Container Widget in this context is a widget that contains Managed Widgets.
 * This is used to easily push overlays onto the screen with custom behavior.
 *
 * For instance, gameplay widgets like Stamina, Interaction, etc. when pushed to the screen
 * will overlay rather than replace and will also get collectively hidden when a pause menu
 * for instance is opened.
 *
 * Menu widgets such as Inventory, Pause, Settings, will replace the last pushed widget rather
 * than overlaying, utilizing CommonUI's Widget Stack.
 *
 * This allows for easy management of what gets hidden, what doesn't, and what widgets get
 * re-pushed when you exit menus. (i.e. closing Settings may bring you back to the Pause screen)
 */
UCLASS(Abstract)
class TORORUNTIME_API UToroContainerWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UToroContainerWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), ZOrder(100)
	{}

	void ClearNullEntries();
	virtual bool PushEntry(UToroManagedWidget* Widget);
	virtual bool PopEntry(UToroManagedWidget* Widget);

protected:

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		uint8 ZOrder;

	TArray<TWeakObjectPtr<UToroManagedWidget>> ManagedWidgets;

	virtual void InitWidget(APlayerController* Controller) override;
};

UCLASS(Abstract)
class TORORUNTIME_API UToroOverlayContainer : public UToroContainerWidget
{
	GENERATED_BODY()

public:

	UToroOverlayContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{}

	virtual bool PushEntry(UToroManagedWidget* Widget) override;
	virtual bool PopEntry(UToroManagedWidget* Widget) override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UOverlay> WidgetStack;

	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UToroStackContainer : public UToroContainerWidget
{
	GENERATED_BODY()

public:

	UToroStackContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{}

	virtual bool PushEntry(UToroManagedWidget* Widget) override;
	virtual bool PopEntry(UToroManagedWidget* Widget) override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetStack> WidgetStack;

	virtual void NativePreConstruct() override;
};
