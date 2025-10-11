// Copyright YING


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
	//const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec =  FGameplayAbilitySpec(Ability,1);
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
		if(!InputTag.IsValid()) return;
    	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    	{
    		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
    		{
    			AbilitySpecInputReleased(AbilitySpec);
    		}
    	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag InputTag)
{
		if(!InputTag.IsValid()) return;
    	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    	{
    		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
    		{
    			AbilitySpecInputPressed(AbilitySpec);
    			if (!AbilitySpec.IsActive())
    			{
    				TryActivateAbility(AbilitySpec.Handle);
    			}
    		}
    	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);

}
