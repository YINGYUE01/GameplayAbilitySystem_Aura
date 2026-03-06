// Copyright YING


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	
	const int32 ScaleDamage =Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"
			"<Small>Level:</><Level>%d</>\n"
			"<Small>ManaCost:</><ManaCost>%.1f</>\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>\n"
			"<Default>Launches a bolt of fire,"
			"exploding on impact and dealing: </>"
			"<Damage>%d</><Default> fire damage</>"),Level,ManaCost,Cooldown,ScaleDamage);
	}
	else
	{
		return FString::Printf(TEXT(
		"<Title>FIRE BOLT</>\n"
		"<Small>Level:</><Level>%d</>\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>\n"
		"<Default>Launches %d bolt of fire,"
		"exploding on impact and dealing: </>"
		"<Damage>%d</><Default> fire damage</>"),Level,ManaCost,Cooldown,FMath::Min(Level,NumFireBolts),ScaleDamage);
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const int32 ScaleDamage =Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
	"<Title>NEXT LEVEL</>\n"
	"<Small>Level:</><Level>%d</>\n"
	"<Small>ManaCost:</><ManaCost>%.1f</>\n"
	"<Small>Cooldown:</><Cooldown>%.1f</>\n"
	"<Default>Launches %d bolt of fire,"
	"exploding on impact and dealing: </>"
	"<Damage>%d</><Default> fire damage</>"),Level,ManaCost,Cooldown,FMath::Min(Level,NumFireBolts),ScaleDamage);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation,const FGameplayTag& SocketTag,bool ShouldPitchOverride,float PitchOverride)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		FTransform SpawnTransform;
		const FVector CombatSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),SocketTag);
		SpawnTransform.SetLocation(CombatSocketLocation);
		FRotator Direction = (TargetLocation - SpawnTransform.GetLocation()).Rotation();
		//Direction.Pitch = 0.f;
		if (ShouldPitchOverride)
		{
			Direction.Pitch = PitchOverride;
		}
		SpawnTransform.SetRotation(Direction.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo())
			,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefault();
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::SpawnProjectiles(const FVector& TargetLocation, const FGameplayTag& SocketTag,
	bool ShouldPitchOverride, float PitchOverride,AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FTransform SpawnTransform;
	const FVector CombatSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),SocketTag);
	SpawnTransform.SetLocation(CombatSocketLocation);
	FRotator Rotation = (TargetLocation - SpawnTransform.GetLocation()).Rotation();
	//Direction.Pitch = 0.f;
	if (ShouldPitchOverride)
	{
		Rotation.Pitch = PitchOverride;
	}
	if (CombatInterface)
	{
		const int32 NumProjectiles = FMath::Min(NumFireBolts,GetAbilityLevel());
		FVector Forward = Rotation.Vector();
		TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlySpacedVectors(Forward,FVector::UpVector,NumProjectiles,ProjectileSpread);
		for (FVector& Direction : Directions)
		{
			SpawnTransform.SetRotation(Direction.Rotation().Quaternion());
			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
				 SpawnTransform,
		  GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo())
				 ,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefault();
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

