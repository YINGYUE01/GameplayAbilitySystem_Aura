// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"
class UNiagaraSystem;
class UAnimMontage;
// This class does not need to be modified.
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UAnimMontage* Montage=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MontageTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag SocketTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	USoundBase* HitSound=nullptr;
};
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetPlayerLevel();
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag MontageTag);
	//蓝图可实现事件
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitMontage();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool bIsDead() const;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();
	virtual void Die() = 0;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetMinionCount();
};
