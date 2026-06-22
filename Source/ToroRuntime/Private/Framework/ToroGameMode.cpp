// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerState.h"
#include "Framework/ToroPlayerHUD.h"
#include "Framework/ToroGameState.h"

AToroGameMode::AToroGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	GameStateClass = AToroGameState::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	HUDClass = AToroPlayerHUD::StaticClass();

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}
