// Copyright YING


#include "AbilitySystem/Ability/ArcaneShardsAbility.h"

FString UArcaneShardsAbility::GetDescription(int32 Level)
{
	const float ScaleDamage =Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS</>\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>ManaCost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n"
			"<Default>Summon one spike traps,"
			"exploding on impact and dealing: </>"
			"<Damage>%.1f</><Default> Arcane damage</>"),Level,ManaCost,Cooldown,ScaleDamage);
	}
	else
	{
		return FString::Printf(TEXT(
		"<Title>ARCANE SHARDS</>\n"
		"<Small>Level:</><Level>%d</>\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>\n"
		"<Default>Summon %d spike traps,"
		"exploding on impact and dealing: </>"
		"<Damage>%.1f</><Default> Arcane damage</>"),Level,ManaCost,Cooldown,FMath::Min(Level,NumPoints),ScaleDamage);
	}
}

FString UArcaneShardsAbility::GetNextLevelDescription(int32 Level)
{
	const float ScaleDamage =Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
	"<Title>NEXT LEVEL</>\n"
	"<Small>Level:</><Level>%d</>\n"
	"<Small>ManaCost:</><ManaCost>%.1f</>\n"
	"<Small>Cooldown:</><Cooldown>%.1f</>\n"
	"<Default>Summon %d spike traps,"
	"exploding on impact and dealing: </>"
	"<Damage>%.1f</><Default> Arcane damage</>"),Level,ManaCost,Cooldown,FMath::Min(Level,NumPoints),ScaleDamage);
}
