// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Components/PointLightComponent.h"
#include "Interaction/InteractionManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Libraries/ToroLightingUtils.h"
#include "Libraries/ToroMathLibrary.h"
#include "Framework/ToroGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "ToroRuntime.h"

namespace PlayerLockTags
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Gameplay)
}

AToroPlayerCharacter::AToroPlayerCharacter()
	: SlowTickInterval(0.1f), LockTags({PlayerLockTags::TAG_Loading.GetTag()})
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	CharacterID = CharacterTags::TAG_Player;

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetupAttachment(GetCapsuleComponent());

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bDoCollisionTest = false;
	CameraArm->TargetArmLength = 0.0f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>("EquipmentRoot");
	EquipmentRoot->SetRelativeLocation(FVector(22.0f, 20.0f, -15.0f));
	EquipmentRoot->SetupAttachment(PlayerCamera);
#if WITH_EDITORONLY_DATA
	EquipmentRoot->bVisualizeComponent = true;
#endif

	Interaction = CreateDefaultSubobject<UInteractionManager>("Interaction");

	CharacterID = CharacterTags::TAG_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}

bool AToroPlayerCharacter::IsControlLocked()
{
	if (!LockTags.IsEmpty())
	{
		return true;
	}
	if (UToroSettings::Get()->IsOnMap(this, EToroMapType::MainMenu))
	{
		return true;
	}
	if (const AToroPlayerController* PC = GetPlayerController(); PC && PC->bCinematicMode)
	{
		return true;
	}
	return false;
}

bool AToroPlayerCharacter::IsKillLocked()
{
	if (const UToroGameInstance* GI = GetGameInstance<UToroGameInstance>())
	{
		return GI->IsPlayerInvincible();
	}
	if (const AToroPlayerController* PC = GetPlayerController(); PC && PC->bCinematicMode)
	{
		return true;
	}
	return false;
}

void AToroPlayerCharacter::AddLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag))
	{
		LockTags.AddTag(InTag);
	}
}

void AToroPlayerCharacter::RemoveLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag))
	{
		LockTags.RemoveTag(InTag);
	}
}

void AToroPlayerCharacter::ClearLockTags()
{
	LockTags.Reset();
}

void AToroPlayerCharacter::SetLockOnTarget(UObject* InTarget)
{
	if (!InTarget)
	{
		ClearLockOnTarget();
	}
	LockOnTarget = InTarget;
}

void AToroPlayerCharacter::ClearLockOnTarget()
{
	LockOnTarget.Reset();
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::PlayFootstep(USoundBase* InSound)
{
	if (InSound)
	{
		FootstepAudio->SetSound(InSound);
		FootstepAudio->Play();
	}
}

bool AToroPlayerCharacter::GetStandingSurface(TEnumAsByte<EPhysicalSurface>& Surface, const ECollisionChannel TraceChannel)
{
	FVector Start, End;
	UToroMathLibrary::GetComponentLineTraceVectors(FootstepAudio, EVectorDirection::Up,
		-(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 16.0f), Start, End);
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel,
		FCollisionQueryParams("PlayerTrace_Floor", true, this)))
	{
		Surface = (!Hit.PhysMaterial.IsValid()) ? SurfaceType_Default
			: UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		return true;
	}

	Surface = SurfaceType_Default;
	return false;
}

void AToroPlayerCharacter::SetControlRotation(const FRotator& InRotator) const
{
	if (GetController()) GetController()->SetControlRotation(InRotator);
}

bool AToroPlayerCharacter::GetViewTarget_Implementation(FVector& Location) const
{
	if (LockOnTarget.IsValid())
	{
		Location = ICharInterface::GetFocusPoint(LockOnTarget.Get());
		return true;
	}
	return false;
}

void AToroPlayerCharacter::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = PlayerCamera->GetComponentLocation();
	Forward = PlayerCamera->GetForwardVector();
	Angle = PlayerCamera->FieldOfView;
}

void AToroPlayerCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	const APlayerController* PC = GetPlayerController();
	if (APlayerCameraManager* CM = PC ? PC->PlayerCameraManager : nullptr)
	{
		CM->SetGameCameraCutThisFrame();
	}

	SetActorLocation(InLocation, false, nullptr, ETeleportType::ResetPhysics);

	const bool bSmooth = CameraArm->bEnableCameraRotationLag;
	CameraArm->bEnableCameraRotationLag = false;

	FRotator Rot(InRotation); Rot.Roll = 0.0f;
	Controller->SetControlRotation(Rot);

	CameraArm->bEnableCameraRotationLag = bSmooth;
}

void AToroPlayerCharacter::OnSettingsUpdate(const ESettingApplyType ApplyType)
{
	if (const UToroUserSettings* UserSettings = UToroUserSettings::Get())
	{
		CameraArm->bEnableCameraRotationLag = UserSettings->GetSmoothCamera();
	}
}

float AToroPlayerCharacter::GetCapsuleCameraOffset() const
{
	// return FMath::Lerp(
	// 	GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere(),
	// 	GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(),
	// 	CapLerp);
	return GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere();
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnSettingsUpdate(ESettingApplyType::Manual);
	if (UToroUserSettings* UserSettings = UToroUserSettings::Get())
	{
		UserSettings->OnSettingsUpdated.AddUObject(this, &AToroPlayerCharacter::OnSettingsUpdate);
	}
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		if (AToroPlayerController* PC = GetPlayerController())
		{
			PC->SetViewTarget(this);
		}
	});
}

void AToroPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SlowTickTime >= SlowTickInterval)
	{
		SlowTick();
		SlowTickTime = 0.0f;
	}
	else SlowTickTime += DeltaTime;
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
	LockTags.RemoveTag(PlayerLockTags::TAG_PlayerLock);
	CameraArm->SetRelativeLocation({
		0.0f, 0.0f, GetCapsuleCameraOffset()
	});
}
