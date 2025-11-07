// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "ToroWidgetManager.generated.h"

UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AToroWidgetManager : public AHUD
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		const APlayerController* PC = UGameplayStatics::GetPlayerController(ContextObject, PlayerIdx);
		return PC ? Cast<T>(PC->GetHUD()) : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = WidgetManager, meta = (DeterminesOutputType = "WidgetClass"))
		UToroWidgetBase* FindOrAddWidget(const TSubclassOf<UToroWidgetBase> WidgetClass);

	UFUNCTION(BlueprintPure, Category = WidgetManager, meta = (DeterminesOutputType = "WidgetClass"))
		UToroWidgetBase* FindWidget(const TSubclassOf<UToroWidgetBase> WidgetClass);

	template<typename T = UToroWidgetBase>
	T* FindWidget()
	{
		return Cast<T>(FindWidget(T::StaticClass()));
	}

protected:

	UPROPERTY(Transient)
		TSet<TObjectPtr<UToroWidgetBase>> WidgetObjects;

	virtual void BeginPlay() override;

public: // Statics

	template<typename T = UToroWidgetBase>
	static T* GetWidget(const UObject* ContextObject)
	{
		AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
		return Manager ? Manager->FindWidget<T>() : nullptr;
	}
};
