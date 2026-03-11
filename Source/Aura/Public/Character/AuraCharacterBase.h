// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraCharacterBase.generated.h"
class UDebuffNiagaraComponent;
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly,Category="Anim")
	TObjectPtr<UAnimMontage> HitReactMontage;
	/* CombatInterface*/
	virtual UAnimMontage* GetHitMontage_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) override;
	virtual void Die(const FVector DeathImpulse) override;
	virtual bool bIsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual bool IsDead_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterCLass_Implementation() override;
	virtual FOnASCRegister& GetOnASCRegisterDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeaponMesh_Implementation() override;
	virtual bool IsBeingShocked_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(const bool bInShock) override;
	/*CombatInterface*/
	FOnASCRegister OnAscRegister;
	FOnDeath OnDeath;
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath(const FVector DeathImpulse);

	UPROPERTY(ReplicatedUsing=OnRep_Stuned,BlueprintReadOnly)
	bool Stunned = false;
	UPROPERTY(ReplicatedUsing=OnRep_Burned,BlueprintReadOnly)
	bool Burned = false;

	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bIsBeingShocked = false;
	UFUNCTION()
	virtual void OnRep_Stuned();
	UFUNCTION()
	virtual void OnRep_Burned();
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	UPROPERTY(EditAnywhere,Category="Combat")
	UNiagaraSystem* BloodEffect;
	UPROPERTY(EditAnywhere,Category="Combat")
	USoundBase* DeathSound;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffNiagaraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffNiagaraComponent;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 600.f;

	virtual void StunTagChanged(const FGameplayTag CallbackTag,int32 NewCount);
	virtual void BurnTagChanged(const FGameplayTag CallbackTag,int32 NewCount);

	//ASC
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>	AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	//ASC
	virtual void InitAbilityActorInfo();
	//召唤数量限制
	int32 MinionCount = 0;
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
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
};
