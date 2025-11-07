// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/RadialProgressBar.h"

URadialProgressBar::URadialProgressBar(): Progress(0.6f), Parameter("Progress")
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Visibility = ESlateVisibility::HitTestInvisible;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
#if WITH_EDITOR
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(
		TEXT("/ToroUtilities/Assets/ProgressBars/M_RadialBar_Spheric.M_RadialBar_Spheric"));
	if (MaterialFinder.Succeeded()) BarMaterial = MaterialFinder.Object;
#endif
}

void URadialProgressBar::SetProgress(const float InProgress)
{
	const float Target = FMath::Clamp(InProgress, 0.0f, 1.0f);
	if (!FMath::IsNearlyEqual(Progress, Target))
	{
		Progress = Target;
		UpdateMatInstance();
	}
}

void URadialProgressBar::UpdateMatInstance()
{
	bool bBrushChanged = false;
	if (MatInstance)
	{
		MatInstance->SetScalarParameterValue(Parameter, Progress);
		if (MatInstance != GetBrush().GetResourceObject())
		{
			bBrushChanged = true;
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			Brush.SetResourceObject(MatInstance);
			PRAGMA_ENABLE_DEPRECATION_WARNINGS
		}
	}
	else
	{
		bBrushChanged = true;
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		Brush.SetResourceObject(nullptr);
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	}

	if (bBrushChanged)
	{
		BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::Brush);
		if (MyImage.IsValid())
		{
			MyImage->InvalidateImage();
		}
	}
}

void URadialProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITOR
	if (!BarMaterial)
	{
		BarMaterial = LoadObject<UMaterialInterface>(nullptr,
			TEXT("/ToroUtilities/Assets/ProgressBars/M_RadialBar_Spheric.M_RadialBar_Spheric"));
	}
	if (BarMaterial)
	{
		TArray<FGuid> Ids;
		TArray<FMaterialParameterInfo> Infos; 
		BarMaterial->GetAllScalarParameterInfo(Infos, Ids);

		ScalarParams.Empty(Infos.Num());
		for (int32 i = 0; i < Infos.Num(); i++)
		{
			if (!Infos[i].Name.IsNone() && Ids.IsValidIndex(i) && Ids[i].IsValid())
			{
				ScalarParams.Add(Infos[i].Name);
			}
		}
	}
#endif
	if (BarMaterial && (!MatInstance || MatInstance->Parent != BarMaterial))
	{
		MatInstance = UMaterialInstanceDynamic::Create(BarMaterial, this);
	}
	UpdateMatInstance();
}

void URadialProgressBar::ReleaseSlateResources(bool bWholeView)
{
	Super::ReleaseSlateResources(bWholeView);
	MatInstance = nullptr;
	UpdateMatInstance();
}
