// Copyright YING


#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	for (TTuple<FGameplayTag,FScalableFloat> Pair : DamageTypes)
	{
		const float ScaleDamage =Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageHandle,Pair.Key,ScaleDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
