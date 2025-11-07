// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/EquipmentActor.h"

AEquipmentActor::AEquipmentActor(): bUsing(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AEquipmentActor::OnEquip_Implementation(USceneComponent* EquipmentHandle)
{
	AttachToComponent(EquipmentHandle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AEquipmentActor::OnUnequip_Implementation()
{
	SetActorHiddenInGame(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		K2_DestroyActor();
	});
}

void AEquipmentActor::EquipItem()
{
	PlayerChar = AToroPlayerCharacter::Get(this);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		OnEquip(PlayerChar->EquipmentRoot);
	});
}

void AEquipmentActor::UnequipItem()
{
	SetUseState(false);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		OnUnequip();
	});
}

void AEquipmentActor::SetUseState(const bool bInUse)
{
	if (bUsing != bInUse)
	{
		bUsing = bInUse;
		bUsing ? OnBeginUse() : OnEndUse();
	}
}
