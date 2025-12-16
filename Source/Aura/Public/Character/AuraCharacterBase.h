// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"
class UGameplayAbility;
class UGameplayEffect;
class USkeletalMeshComponent;
class UAttributeSet;
class UAbilitySystemComponent;
class UNiagaraSystem;
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly,Category="Anim")
	TObjectPtr<UAnimMontage> HitReactMontage;
	/* CombatInterface*/
	virtual UAnimMontage* GetHitMontage_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) override;
	virtual void Die() override;
	virtual bool bIsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual bool IsDead_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	/*CombatInterface*/
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	UPROPERTY(EditAnywhere,Category="Combat")
	UNiagaraSystem* BloodEffect;
	UPROPERTY(EditAnywhere,Category="Combat")
	USoundBase* DeathSound;
protected:
	virtual void BeginPlay() override;
	//Weapon Socket
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere,Category="Attack Socket")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere,Category="Attack Socket")
	FName LeftSocketName;
	UPROPERTY(EditAnywhere,Category="Attack Socket")
	FName RightSocketName;
	UPROPERTY(EditAnywhere,Category="Attack Socket")
	FName TailSocketName;
	//Weapon Socket

	bool bDead = false;
	//ASC
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>	AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	//ASC

	virtual void InitAbilityActorInfo();
	//DefaultAttributes
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Ability")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Ability")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Ability")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	//DefaultAttributes
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	//Dissolve Effects
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	//Dissolve Effects
	
private:
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};
