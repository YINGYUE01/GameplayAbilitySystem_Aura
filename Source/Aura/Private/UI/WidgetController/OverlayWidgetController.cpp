// Copyright YING


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitalValues()
{

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}


void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetManaAttribute()).AddUObject(this,&UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this,&UOverlayWidgetController::MaxManaChanged);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FString TagString = FString::Printf(TEXT("GE Tag %s"),*Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Cyan,TagString);
				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
			}
		}
		);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Deta) const
{
	OnHealthChanged.Broadcast(Deta.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Deta) const
{
	OnMaxHealthChanged.Broadcast(Deta.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Deta) const
{
	OnManaChanged.Broadcast(Deta.NewValue);
}
void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Deta) const
{
	OnMaxManaChanged.Broadcast(Deta.NewValue);
}
