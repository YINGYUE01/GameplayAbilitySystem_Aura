// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariable();
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	UFUNCTION(BlueprintCallable)
	void StoreAdditionTargets(TArray<AActor*>& OutAdditionTargets);
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
protected:
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	FVector MouseHitLocation;
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<AActor> MouseHitActor;
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly,Category="Beam")
	int32 MaxNumShockingTargets = 5;


};
