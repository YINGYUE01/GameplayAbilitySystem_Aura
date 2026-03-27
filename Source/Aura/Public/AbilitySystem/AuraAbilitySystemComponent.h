// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven)
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged,const FGameplayTag& /*Ability Tag*/,const FGameplayTag& /*Status Tag*/,int32 /*Ability Level*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEuipped,const FGameplayTag& /*Ability Tag*/,const FGameplayTag& /*Slot Tag*/,const FGameplayTag& /*Status Tag*/,const FGameplayTag& /*PreSlot Tag*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility,const FGameplayTag&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect,const FGameplayTag&,bool);
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEuipped AbilityEquippedDelegate;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FActivatePassiveEffect ActivatePassiveEffect;
	
	void ForEachAbility(const FForEachAbility& Delegate);
	
	bool bStartupAbilitiesGiven = false;

	void AddCharacterAbilitiesFromSaveGame(ULoadScreenSaveGame* SaveGame);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetSlotTagFromAbilityTag(const FGameplayTag& AbilityTag);
	bool SlotIsEmpty(const FGameplayTag& SlotTag);
	FGameplayAbilitySpec* GetSpecWithSLotTag(const FGameplayTag& SlotTag);
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec);
	bool AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec);
	void AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec,const FGameplayTag& SlotTag);

	UFUNCTION(NetMulticast,Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag,bool bActivate);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	void UpgradeAttribute(const FGameplayTag& GameplayTag);

	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& GameplayTag);
	UFUNCTION(Server,Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server,Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& SlotTag);

	UFUNCTION(Client,Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& SlotTag,const FGameplayTag& StatusTag,const FGameplayTag& PreSlotTag);
	
	void UpdateAbilityStatuses(int32 Level);

	bool GetDescriptionFromAbilityTag(const FGameplayTag& AbilityTag,FString& OutSpellDescription,FString& NextLevelSpellDescription);

	void ClearSlot(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilityOfSlotTag(const FGameplayTag& SlotTag);
	static bool AbilityHasSlotTag(const FGameplayAbilitySpec* AbilitySpec,const FGameplayTag& SlotTag);
protected:
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const;
	
	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client,Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 AbilityLevel);
	
};




