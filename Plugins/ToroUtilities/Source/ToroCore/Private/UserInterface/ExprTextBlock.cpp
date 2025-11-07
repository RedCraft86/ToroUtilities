// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ExprTextBlock.h"
#include "Asset/ExpressiveTextAsset.h"
#include "Styles/ExpressiveTextStyle.h"

#define GetExprDefault LoadObject<UExpressiveTextStyle>(nullptr, \
	TEXT("/Game/AssetPacks/ExpressiveText/TextStyles/ETS_Default.ETS_Default"));

FExpressiveTextData::FExpressiveTextData(): bUseAsset(false)
{
	TextFields.DefaultFontSize = 24;
	TextFields.UseDefaultFontSize = true;
	TextFields.WrapSettings.Value = 1.0f;
	TextFields.WrapSettings.ValueType = EExpressiveTextWrapMode::WrapAtPercentageOfParentSize;
	TextFields.DefaultStyle = GetExprDefault;
}

int64 FExpressiveTextData::CalcChecksum() const
{
	int64 Result = TextFields.CalcChecksum();
	Result = HashCombine(Result, GetTypeHash(bUseAsset));
	Result = HashCombine(Result, GetTypeHash(TextAsset));
	if (TextAsset) Result = HashCombine(Result, TextAsset->CalcChecksum());
	return Result;
}

FExpressiveText FExpressiveTextData::GetExpressiveText()
{
	FExpressiveText Result;
	if (bUseAsset)
	{
		ensureAlwaysMsgf(TextAsset, TEXT("[ExprTextTypes.cpp:31] Cannot use TextAsset, it is NULL"));
		if (TextAsset) Result.SetFields(TextAsset->Fields);
	}
	else
	{
		Result.SetFields(TextFields);
		if (!Result.GetDefaultStyle())
		{
			TextFields.DefaultStyle = GetExprDefault;
			Result.SetDefaultStyle(TextFields.DefaultStyle);
		}
	}
	
	return Result;
}

void FExpressiveTextData::SetText(const FText& InText, const bool bUseFieldsFromAsset)
{
	if (bUseFieldsFromAsset && TextAsset) SetTextFields(TextAsset->Fields);
	TextFields.Text = InText;
	bUseAsset = false;
}

void FExpressiveTextData::SetTextFields(const FExpressiveTextFields& InFields)
{
	bUseAsset = false;
	TextFields = InFields;
}

void FExpressiveTextData::SetTextAsset(UExpressiveTextAsset* InAsset)
{
	bUseAsset = true;
	TextAsset = InAsset;
}

void UExprTextBlock::SetText(const FText& InText, const bool bUseFieldsFromAsset)
{
	ExpressiveText.SetText(InText, bUseFieldsFromAsset);
	UpdateText();
}

void UExprTextBlock::SetTextFields(const FExpressiveTextFields& InFields)
{
	ExpressiveText.SetTextFields(InFields);
	UpdateText();
}

void UExprTextBlock::SetTextAsset(UExpressiveTextAsset* InAsset)
{
	ExpressiveText.SetTextAsset(InAsset);
	UpdateText();
}

void UExprTextBlock::UpdateSize()
{
	CurrentSize = GetCachedGeometry().GetLocalSize();
	if (IsCacheInvalid()) UpdateText();
}

void UExprTextBlock::UpdateText()
{
	CachedSize = CurrentSize;
	CachedChecksum = ExpressiveText.CalcChecksum();
	FExpressiveText GenExprText = ExpressiveText.GetExpressiveText();
	GenExprText.DefineWorldContext(this);
	SetExpressiveText(GenExprText);
}

bool UExprTextBlock::IsCacheInvalid() const
{
	return CachedChecksum != ExpressiveText.CalcChecksum()
		|| !CachedSize.Equals(CurrentSize, 0.001f);
}

void UExprTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	CurrentSize = GetCachedGeometry().GetLocalSize();
	UpdateText();
}
