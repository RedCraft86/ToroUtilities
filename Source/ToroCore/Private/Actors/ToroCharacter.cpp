// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Actors/ToroCharacter.h"

AToroCharacter::AToroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);

	UniqueId.RegenerateGuid();
}

bool AToroCharacter::SetControlRotation(const FRotator& Rotation, const bool bIncludeRoll) const
{
	if (AController* Cont = GetController())
	{
		Cont->SetControlRotation(FRotator(Rotation.Pitch, Rotation.Yaw, bIncludeRoll ? Rotation.Roll : 0.0f));
		return true;
	}

	return false;
}

void AToroCharacter::Teleport(const FVector& Location, const FRotator& Rotation)
{
	SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
	if (!SetControlRotation(Rotation, false))
	{
		SetActorRotation(FRotator(Rotation.Pitch, Rotation.Yaw, 0.0f), ETeleportType::TeleportPhysics);
	}
}

void AToroCharacter::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const
{
	Location = GetPawnViewLocation();
	Forward = GetViewRotation().Vector();
	Angle = 75.0f;
}

#if WITH_EDITOR
void AToroCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UniqueId.RegenerateGuid(true);
}
#endif