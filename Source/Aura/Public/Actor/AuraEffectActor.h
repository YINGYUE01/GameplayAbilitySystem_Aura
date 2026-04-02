// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


class UAbilitySystemComponent;
class UGameplayEffect;

UENUM()
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyEndOverlap,
	DoNotApply
};

UENUM()
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadOnly)
	FVector CalculateLocation;
	UPROPERTY(BlueprintReadOnly)
	FRotator CalculateRotation;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Pick Up Movement")
	bool bRotates = false;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Pick Up Movement")
	float RotationRate = 45.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Pick Up Movement")
	bool bSinusoidalMovement = false;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Pick Up Movement")
	float SinAmplitude = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Pick Up Movement")
	float SinPeriodConstant = 1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Pick Up Movement")
	FVector InitialLocation;
	UFUNCTION(BlueprintCallable)
	void StartSinusodalMovement();

	UFUNCTION(BlueprintCallable)
	void StartRotation();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	bool bDestroyOnEffectApplication=false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	bool bApplyEffectsToEnemies=false;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy=EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effects")
	float ActorLevel=1.0f;
private:
	float RunningTime = 0.f;
	void ItemMovement(float DeltaTime);
};
