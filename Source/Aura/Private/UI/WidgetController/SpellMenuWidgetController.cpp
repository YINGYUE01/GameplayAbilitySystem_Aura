// Copyright YING


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 NewLevel)
	{
		if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;
			bool bShouldEnabledSpellPointsButton = false;
			bool bShouldEnabledEquipButton = false;
			FString OutSpellDescription;
			FString OutNextLevelDescription;
			GetAuraASC()->GetDescriptionFromAbilityTag(AbilityTag,OutSpellDescription,OutNextLevelDescription);
			ShouldEnableButton(StatusTag,CurrentSpellPoints,bShouldEnabledSpellPointsButton,bShouldEnabledEquipButton);
			OnSpellGlobeSelectedDelegate.Broadcast(bShouldEnabledSpellPointsButton,bShouldEnabledEquipButton,OutSpellDescription,OutNextLevelDescription);
		}
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChangedDelegate.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;
		bool bShouldEnabledSpellPointsButton = false;
		bool bShouldEnabledEquipButton = false;
		FString OutSpellDescription;
		FString OutNextLevelDescription;
		GetAuraASC()->GetDescriptionFromAbilityTag(SelectedAbility.AbilityTag,OutSpellDescription,OutNextLevelDescription);
		ShouldEnableButton(SelectedAbility.StatusTag,CurrentSpellPoints,bShouldEnabledSpellPointsButton,bShouldEnabledEquipButton);
		OnSpellGlobeSelectedDelegate.Broadcast(bShouldEnabledSpellPointsButton,bShouldEnabledEquipButton,OutSpellDescription,OutNextLevelDescription);
	});
}

void USpellMenuWidgetController::OnSpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelections)
	{
		const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquipSelectDelegate.Broadcast(AbilityType);
		bWaitingForEquipSelections = false;
	}
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayTag StatusTag;
	int32 SpellPoint = GetAuraPS()->GetSpellPoints();
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec!=nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		StatusTag = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		StatusTag = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}
	SelectedAbility.StatusTag = StatusTag;
	SelectedAbility.AbilityTag = AbilityTag;
	bool bShouldEnabledSpellPointsButton = false;
	bool bShouldEnabledEquipButton = false;
	FString OutSpellDescription;
	FString OutNextLevelDescription;
	GetAuraASC()->GetDescriptionFromAbilityTag(AbilityTag,OutSpellDescription,OutNextLevelDescription);
	ShouldEnableButton(StatusTag,SpellPoint,bShouldEnabledSpellPointsButton,bShouldEnabledEquipButton);
	OnSpellGlobeSelectedDelegate.Broadcast(bShouldEnabledSpellPointsButton,bShouldEnabledEquipButton,OutSpellDescription,OutNextLevelDescription);
	
}

void USpellMenuWidgetController::SpendPointButton()
{
	GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelections)
	{
		const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquipSelectDelegate.Broadcast(AbilityType);
		bWaitingForEquipSelections = false;
	}
	SelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	OnSpellGlobeSelectedDelegate.Broadcast(false,false,FString(),FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	bWaitingForEquipSelections =  true;
	WaitForEquipSelectDelegate.Broadcast(AbilityType);
}

void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& StatusTag, int32 SpellPoints,
                                                    bool& bShouldEnabledSpellPointsButton, bool& bShouldEnabledEquipButton)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bShouldEnabledSpellPointsButton = false;
	if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints>0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
		
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnabledEquipButton = false;
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_UnLocked))
	{
		bShouldEnabledEquipButton = true;
		if (SpellPoints>0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnabledEquipButton = false;
		if (SpellPoints>0)
		{
			bShouldEnabledSpellPointsButton = true;
		}
	}
}
