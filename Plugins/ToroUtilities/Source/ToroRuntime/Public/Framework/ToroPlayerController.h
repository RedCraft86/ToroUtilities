// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EnhancedInputSubsystems.h"
#include "DataTypes/GameInputConfig.h"
#include "Helpers/ClassGetterMacros.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerController.generated.h"

UCLASS()
class TORORUNTIME_API AToroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AToroPlayerController();
	
	PLAYER_CLASS_GETTER(AToroPlayerController, GetPlayerController)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void SetInputConfig(const FGameInputConfig& InputConfig);

	UFUNCTION(BlueprintPure, Category = Input)
		const FGameInputConfig& GetInputConfig() const { return CachedInputConfig; }

	UFUNCTION(BlueprintCallable, Category = Controller)
		virtual void SetUserPause(const bool bShouldPause);

	UFUNCTION(BlueprintPure, Category = Controller)
		bool IsUserPaused() const { return bUserPaused; }

	UFUNCTION(BlueprintCallable, Category = Controller)
		virtual void AddPauseRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = Controller)
		virtual void RemovePauseRequest(const UObject* InObject);

	UFUNCTION(BlueprintPure, Category = Controller)
		AActor* GetCinematicActor() const { return CinematicActor.Get(); }

	UFUNCTION(BlueprintCallable, Category = Controller)
		virtual void ExitCinematic();
	
	UFUNCTION(BlueprintCallable, Category = Controller)
		virtual void EnterCinematic(AActor* InActor);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnyKeyPressedBP, const FKey&, Key);
	UPROPERTY(BlueprintAssignable)
		FOnAnyKeyPressedBP OnAnyKeyPressedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnyKeyPressed, const FKey&);
	FOnAnyKeyPressed OnAnyKeyPressed;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUserPaused, const bool)
	FOnUserPaused OnUserPaused;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameFocusChanged, const bool);
	FOnGameFocusChanged OnGameFocusChanged;

protected:

	bool bUserPaused;
	FGameInputConfig CachedInputConfig;
	TWeakObjectPtr<AActor> CinematicActor;
	TSet<TWeakObjectPtr<const UObject>> PauseRequests;

	virtual void UpdatePauseState();
	virtual void OnWindowFocusChanged(bool bFocused);
	virtual void OnAnyKeyEvent(FKey PressedKey);

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
