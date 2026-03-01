// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellGlobeSelected,bool,SpendPoint/*Button SpendPoint*/,bool,Equip/*Button Equip*/);

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
	
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;
	UFUNCTION(BlueprintCallable)
	void OnSpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButton();
private:
	void ShouldEnableButton(const FGameplayTag& StatusTag,int32 SpellPoints,bool& bShouldEnabledSpellPointsButton,bool& bShouldEnabledEquipButton);
	FSelectedAbility SelectedAbility = {FAuraGameplayTags::Get().Abilities_None,FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints = 0;
};
