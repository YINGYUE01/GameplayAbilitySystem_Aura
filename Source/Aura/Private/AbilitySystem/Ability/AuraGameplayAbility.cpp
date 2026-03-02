// Copyright YING


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Interfaces/IPluginManager.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s </><Level>%d </>"),L"Level",Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level </><Level>%d</>\n<Default>Create More Damage</>"),Level);
}

float UAuraGameplayAbility::GetManaCost(float Level)
{
	float ManaCost=0.f;
	 if (UGameplayEffect* CostEffect = GetCostGameplayEffect())
	 {
		 for (FGameplayModifierInfo ModifierInfo : CostEffect->Modifiers)
		 {
			 if (ModifierInfo.Attribute==UAuraAttributeSet::GetManaAttribute())
			 {
			 	
			 	ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level,ManaCost);
			 	break;
			 }
		 }
	 }
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float Level)
{
	float Cooldown=0.f;
	if (UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level,Cooldown);
	}
	return Cooldown;
}

FString UAuraGameplayAbility::GetLockedDescription(int32 LevelRequirement)
{
	return FString::Printf(TEXT("<Default>Locked Untiled </><Level>%d</>"),LevelRequirement);
}
