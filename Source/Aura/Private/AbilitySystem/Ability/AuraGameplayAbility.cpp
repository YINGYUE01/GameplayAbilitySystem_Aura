// Copyright YING


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s </><Level>%d </>"),L"Level",Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level </><Level>%d</>\n<Default>Create More Damage</>"),Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 LevelRequirement)
{
	return FString::Printf(TEXT("<Default>Locked Untiled </><Level>%d</>"),LevelRequirement);
}
