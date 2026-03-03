// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellGlobeSelected,bool,SpendPoint/*Button SpendPoint*/,bool,Equip/*Button Equip*/,FString,OutSpellDescription,FString,OutNextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectSignature,const FGameplayTag&,AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassigned,const FGameplayTag&,AbilityTag);
struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnPlayStateChangedSignature SpellPointsChangedDelegate;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnSpellGlobeSelected OnSpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FWaitForEquipSelectSignature WaitForEquipSelectDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FWaitForEquipSelectSignature StopWaitForEquipSelectDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FSpellGlobeReassigned SpellGlobeReassignedDelegate;
	
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;
	UFUNCTION(BlueprintCallable)
	void OnSpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButton();

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag,const FGameplayTag& AbilityType);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag,const FGameplayTag& SlotTag,const FGameplayTag& StatusTag,const FGameplayTag& PreSlotTag);
private:
	void ShouldEnableButton(const FGameplayTag& StatusTag,int32 SpellPoints,bool& bShouldEnabledSpellPointsButton,bool& bShouldEnabledEquipButton);
	FSelectedAbility SelectedAbility = {FAuraGameplayTags::Get().Abilities_None,FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelections = false;
	FGameplayTag SelectSlotTag;
};
