// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"
class UGameplayEffect;
class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{

public:
	GENERATED_BODY()
	/*  可创建子类FireBoltProjectile再重写 */
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	/*  可创建子类FireBoltProjectile再重写 */
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& TargetLocation,const FGameplayTag& SocketTag,bool ShouldPitchOverride=false,float PitchOverride = 10.f);

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& TargetLocation,const FGameplayTag& SocketTag,bool ShouldPitchOverride,float PitchOverride,AActor* HomingTarget);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UPROPERTY(EditDefaultsOnly)
	int32 NumFireBolts = 5;
	
	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMin = 1600.f;
	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMax = 3200.f;
};
