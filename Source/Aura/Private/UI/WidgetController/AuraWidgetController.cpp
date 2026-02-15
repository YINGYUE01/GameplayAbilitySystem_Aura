// Copyright YING


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	const FString Side = GetAuraPC()->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT");
    
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) 
	{
		UE_LOG(LogAura, Warning, TEXT("[%s] Broadcast aborted: Abilities not given yet."), *Side);
		return;
	}
		if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;
    	FForEachAbility BroadcastDelegate;
    	BroadcastDelegate.BindLambda([this,Side](const FGameplayAbilitySpec& AbilitySpec)
    	{
    		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
    		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
    		AbilityInfoDelegate.Broadcast(Info);
    		UE_LOG(LogAura, Log, TEXT("[%s] Broadcasting Info for Ability Tag: %s ,InputTag:%s"), *Side, *Info.AbilityTag.ToString(),*Info.InputTag.ToString());
    	});
    	GetAuraASC()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
