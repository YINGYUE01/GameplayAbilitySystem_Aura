// Copyright YING


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"



void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	


}
void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatInterface->GetCombatSocketLocation());
		FRotator Direction = (TargetLocation - SpawnTransform.GetLocation()).Rotation();
		Direction.Pitch = 0.f;
		SpawnTransform.SetRotation(Direction.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo())
			,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle DamageEffectHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,1,SourceASC->MakeEffectContext());
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		const float ScaleDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		//GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red,FString::Printf(TEXT("Fire Damage %f"),ScaleDamage));
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectHandle,GameplayTags.Damage,ScaleDamage);
		
		Projectile->DamageEffectSpecHandle = DamageEffectHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}