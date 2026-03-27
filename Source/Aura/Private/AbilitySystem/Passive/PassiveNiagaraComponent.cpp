// Copyright YING


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC)
{
	if (AuraASC->bStartupAbilitiesGiven)
	{
		if (AuraASC->GetStatusTagFromAbilityTag(PassiveSpellTag).MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
		{
			Activate();
		}
	}
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivatePassiveEffect.AddUObject(this,&UPassiveNiagaraComponent::OnPassiveActivated);
		ActivateIfEquipped(AuraASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisterDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
			{
				AuraASC->ActivatePassiveEffect.AddUObject(this,&UPassiveNiagaraComponent::OnPassiveActivated);
				ActivateIfEquipped(AuraASC);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivated(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
