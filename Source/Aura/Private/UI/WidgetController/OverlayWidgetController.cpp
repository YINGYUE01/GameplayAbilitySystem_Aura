// Copyright YING


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
	
}


void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
	[this](int32 NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(NewLevel);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	}
		);
	if (GetAuraASC())
	{
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::BroadcastAbilityInfo);
		}
		GetAuraASC()->EffectAssetTags.AddLambda(
        		[this](const FGameplayTagContainer& AssetTags)
        		{
        			for (const FGameplayTag& Tag : AssetTags)
        			{
        				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
        				if (Tag.MatchesTag(MessageTag))
        				{
        					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
        					MessageWidgetRowDelegate.Broadcast(*Row);
        				}
        			}
        		}
        		);
	}
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP)
{
	ULevelUpInfos* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo,TEXT("Unabled to find LevelUpInfo.Please fill out AuraPlayerState Blueprint"))
	const int Level = LevelUpInfo->GetLevelForXP(NewXP);
	const int MaxLevel = LevelUpInfo->LevelUpInformation.Num()-1;
	if (Level<=MaxLevel && Level>0)
	{
		const int32 LevelUpRequiredXP = LevelUpInfo->LevelUpInformation[Level].LevelUpRequiredXP; //到当前等级所需经验
		const int32 PreLevelUpRequiredXP = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequiredXP;//到前一等级所需经验
		
		const int32 ThisLevelRequiredXP = LevelUpRequiredXP - PreLevelUpRequiredXP; //在当前等级基础上升级所需经验值
		const int32 XPForThisLevel = NewXP - PreLevelUpRequiredXP;   // 在当前等级基础上已获得的经验
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(ThisLevelRequiredXP); //计算百分比
		OnXPChangedDelegate.Broadcast(XPBarPercent);
	}
}
