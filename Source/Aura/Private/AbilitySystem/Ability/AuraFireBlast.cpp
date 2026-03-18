// Copyright YING


#include "AbilitySystem/Ability/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

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
	TArray<AAuraFireBall*> FireBalls;
	const FVector ActorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector ActorForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(ActorForward,FVector::UpVector,NumFireBalls,360);
	for (const FRotator& Rotator : Rotators)
	{
		FTransform Transform;
		Transform.SetLocation(ActorLocation);
		Transform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(FireBallClass,
			Transform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(Transform);
	}
	return FireBalls;
}
