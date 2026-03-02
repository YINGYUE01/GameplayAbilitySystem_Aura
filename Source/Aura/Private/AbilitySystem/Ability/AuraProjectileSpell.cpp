// Copyright YING


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"



void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

FString UAuraProjectileSpell::GetDescription(int32 Level)
{

	const FGameplayTag DamageType = FAuraGameplayTags::Get().Get().Damage_Fire;
	const int32 Damage = DamageTypes[DamageType].GetValueAtLevel(Level);
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
			"<Damage>%d</><Default> fire damage</>"),Level,ManaCost,Cooldown,Damage);
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
		"<Damage>%d</><Default> fire damage</>"),Level,ManaCost,Cooldown,FMath::Min(Level,NumFireBolt),Damage);
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const FGameplayTag DamageType = FAuraGameplayTags::Get().Get().Damage_Fire;
	const int32 Damage = DamageTypes[DamageType].GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs<float>(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
	"<Title>NEXT LEVEL</>\n"
	"<Small>Level:</><Level>%d</>\n"
	"<Small>ManaCost:</><ManaCost>%.1f</>\n"
	"<Small>Cooldown:</><Cooldown>%.1f</>\n"
	"<Default>Launches %d bolt of fire,"
	"exploding on impact and dealing: </>"
	"<Damage>%d</><Default> fire damage</>"),Level,ManaCost,Cooldown,FMath::Min(Level,NumFireBolt),Damage);
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
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		
		TArray<TWeakObjectPtr<AActor>> Targets;
		Targets.Add(Projectile);
		EffectContextHandle.AddActors(Targets);
		
		FHitResult HitResult;
		HitResult.Location = TargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,1,EffectContextHandle);
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		for (auto& Pair : DamageTypes)
		{
			const float ScaleDamage =Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle,Pair.Key,ScaleDamage);
		}
		Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
