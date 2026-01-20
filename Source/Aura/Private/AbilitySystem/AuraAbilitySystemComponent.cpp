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
	//赋予角色初始技能
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec =  FGameplayAbilitySpec(Ability,1);
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//给Spec添加标签，因为我们的技能是与标签绑定的 比如 发射火球技能对应的标签就是Ability.FireBolt
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
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
    				//激活技能
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
