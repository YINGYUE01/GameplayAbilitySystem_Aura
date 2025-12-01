// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"
class UAnimMontage;
// This class does not need to be modified.
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
	FVector GetCombatSocketLocation();
	//蓝图可实现事件
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitMontage();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetAttackMontage();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool bIsDead() const;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();
	virtual void Die() = 0;
};
