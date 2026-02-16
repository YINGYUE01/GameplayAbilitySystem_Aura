// Copyright YING


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair : GetAuraAS()->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Value)
	{
		AttributePointsChangedDelegate.Broadcast(Value);
	});
	
	for (auto& Pair : GetAuraAS()->TagsToAttribute)
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& GameplayTag)
{
	GetAuraASC()->UpgradeAttribute(GameplayTag);
	
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute)
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfo(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}
