// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "DataTypes/PrimitiveData.h"
#include "ToroProcGenBase.generated.h"

UENUM(BlueprintType)
enum class EGeneratorLoopMode : uint8
{
	Forward,
	Reverse,
	Random
};

UCLASS(Abstract, HideCategories = (HLOD, Replication, Collision, Physics, Networking, Input), PrioritizeCategories = (Settings, Tools))
class TOROCORE_API AToroProcGenBase : public AActor
{
	GENERATED_BODY()

public:

	AToroProcGenBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
	
	template<typename T>
	T* AddGenericComponent()
	{
		if (T* GeneratedComp = NewObject<T>(this))
		{
			GeneratedComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			GeneratedComp->OnComponentCreated();
			GeneratedComp->RegisterComponent();
			GeneratedComp->SetMobility(EComponentMobility::Movable);
			GeneratedComp->AttachToComponent(GetRootComponent(),
				FAttachmentTransformRules::KeepRelativeTransform);
			return GeneratedComp;
		}
		return nullptr;
	}

protected:

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bRealtimeConstruction;

	UFUNCTION(CallInEditor, Category = Tools)
		virtual void Construct() { EventConstruct(); }

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Construct")
		void EventConstruct();

#if WITH_EDITORONLY_DATA
	/* Remove this source actor after baking */
	UPROPERTY(EditAnywhere, Category = Tools)
		bool bBakeRemoveSource = true;
#endif
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools, DisplayName = "Bake Meshes")
		void BakeMeshes() { BakeInternal(bBakeRemoveSource, true, nullptr); }
#endif
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
public:
	virtual void BakeInternal(const bool bRemoveSource, const bool bSelectActors, TArray<AActor*>* OutActors);
#endif

public: // Statics

	static TArray<int32> LoopMeshArray(const TArray<FTransformMeshData>& Sample,
		const EGeneratorLoopMode Mode, const uint8 Amount);
};
