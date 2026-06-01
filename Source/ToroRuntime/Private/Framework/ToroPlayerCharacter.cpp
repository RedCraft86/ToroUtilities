// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroPlayerCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"

AToroPlayerCharacter::AToroPlayerCharacter(): SlowTickInterval(0.1f), SlowTickTime(1.0f) // Initial run
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	AmbientLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("AmbientLight"));
	AmbientLight->SetupAttachment(GetCapsuleComponent());

	PlayerFootstep = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerFootstep"));
	PlayerFootstep->SetupAttachment(GetCapsuleComponent());
	FixFootstepPosition();

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	LightSettings.Intensity = 0.02f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	LightSettings.ToLightComponent(AmbientLight);

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	LightSettings.ToLightComponent(AmbientLight);
}

void AToroPlayerCharacter::PlayFootstep(USoundBase* InSound, float InVolume)
{
	if (InSound && InVolume > 0.05f)
	{
		PlayerFootstep->SetSound(InSound);
		PlayerFootstep->SetVolumeMultiplier(InVolume);
		PlayerFootstep->Play();
	}
}

bool AToroPlayerCharacter::GetFloorSurface(TEnumAsByte<EPhysicalSurface>& Surface, const ECollisionChannel TraceChannel)
{
	const FVector Start = PlayerFootstep->GetComponentLocation();
	const FVector End = Start - FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	FCollisionQueryParams Params(NAME_None, false, this);
	Params.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params) && Hit.PhysMaterial.IsValid())
	{
		Surface = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		return true;
	}

	Surface = SurfaceType_Default;
	return false;
}

void AToroPlayerCharacter::Teleport(const FVector& InLocation, const FRotator& InRotation)
{
	const APlayerController* PC = GetController<APlayerController>();
	if (APlayerCameraManager* CM = PC ? PC->PlayerCameraManager : nullptr)
	{
		CM->SetGameCameraCutThisFrame();
	}

	Super::Teleport(InLocation, InRotation);
}

void AToroPlayerCharacter::FixFootstepPosition() const
{
	PlayerFootstep->SetRelativeLocation(FVector(0.0f, 0.0f,
		-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight_WithoutHemisphere())
	);
}

void AToroPlayerCharacter::SlowTick()
{
	FixFootstepPosition();
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		if (APlayerController* PC = GetController<APlayerController>())
		{
			PC->SetViewTarget(this);
		}
	});
}

void AToroPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ((SlowTickTime += DeltaTime) >= SlowTickInterval)
	{
		SlowTick();
		SlowTickTime = 0.0f;
	}
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	LightSettings.ToLightComponent(AmbientLight);
#if WITH_EDITOR
	if (!FApp::IsGame()) FixFootstepPosition();
#endif
}
