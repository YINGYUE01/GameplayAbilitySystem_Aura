// Copyright YING


#include "AbilitySystem/Ability/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const float ScaleDamage =Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT</>\n"
		"<Small>Level:</><Level>%d</>\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>\n"
		"<Default>Launches %d bolt of fire,"
		"exploding on impact and dealing: </>"
		"<Damage>%.1f</><Default> fire damage</>"),Level,ManaCost,Cooldown,NumFireBalls,ScaleDamage);

}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const float ScaleDamage =Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
	"<Title>NEXT LEVEL</>\n"
	"<Small>Level:</><Level>%d</>\n"
	"<Small>ManaCost:</><ManaCost>%.1f</>\n"
	"<Small>Cooldown:</><Cooldown>%.1f</>\n"
	"<Default>Launches %d bolt of fire,"
	"exploding on impact and dealing: </>"
	"<Damage>%.1f</><Default> fire damage</>"),Level,ManaCost,Cooldown,NumFireBalls,ScaleDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBall()
{
	return TArray<AAuraFireBall*>();
}
