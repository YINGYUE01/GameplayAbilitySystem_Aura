// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellGlobeSelected,bool,SpendPoint/*Button SpendPoint*/,bool,Equip/*Button Equip*/);
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
private:
	void ShouldEnableButton(const FGameplayTag& StatusTag,int32 SpellPoints,bool& bShouldEnabledSpellPointsButton,bool& bShouldEnabledEquipButton);
};
