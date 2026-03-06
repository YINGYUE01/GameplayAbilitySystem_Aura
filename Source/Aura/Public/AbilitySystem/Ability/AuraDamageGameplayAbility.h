// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefault(AActor* TargetActor = nullptr) const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditAnywhere,Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditAnywhere,Category="Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditAnywhere,Category="Damage")
	float DebuffChance=20.f;

	UPROPERTY(EditAnywhere,Category="Damage")
	float DebuffDamage=5.f;

	UPROPERTY(EditAnywhere,Category="Damage")
	float DebuffDuration=5.f;

	UPROPERTY(EditAnywhere,Category="Damage")
	float DebuffFrequency=1.f;

	UPROPERTY(EditAnywhere,Category="Damage")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditAnywhere,Category="Damage")
	float KnockbackChance = 20.f;

	UPROPERTY(EditAnywhere,Category="Damage")
	float KnockbackImpulseMagnitude = 1000;

	UPROPERTY(EditAnywhere,Category="Damage")
	FVector KnockbackImpulse = FVector::ZeroVector;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);
};
