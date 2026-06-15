// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UE5Coro.h"
#include "CommonUserWidget.h"
#include "CommonActivatableWidget.h"
#include "ToroWidgetBase.generated.h"

/**
 * Base class for user widgets in the ToroUtilities architecture.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	static inline const FText PaletteCategory = NSLOCTEXT("ToroUtilities", "WidgetPaletteCategory", "ToroUtilities");

	UToroUserWidget(const FObjectInitializer& ObjectInit);

	UFUNCTION(BlueprintCallable, Category = UserWidget, DisplayName = "Fade Out & Remove From Parent")
		void FadeOutAndRemoveFromParent();

	virtual void NativeConstruct() override;
#if WITH_EDITOR 
	virtual const FText GetPaletteCategory() override;
#endif // WITH_EDITOR

protected:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidgetAnimOptional))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(EditAnywhere, Category = Activation)
		bool bFadeInOnConstruct;
};

/**
 * Base class for activatable widgets in the ToroUtilities architecture.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroActivatableWidget(const FObjectInitializer& ObjectInit);

	UFUNCTION(BlueprintCallable, Category = UserWidget, DisplayName = "Fade Out & Remove From Parent")
		void FadeOutAndRemoveFromParent();

	virtual void NativeConstruct() override;
#if WITH_EDITOR 
	virtual const FText GetPaletteCategory() override;
#endif // WITH_EDITOR

protected:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidgetAnimOptional))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(EditAnywhere, Category = Activation)
		bool bFadeInOnConstruct;
};
