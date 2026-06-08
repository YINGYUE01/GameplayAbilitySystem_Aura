// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"
struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAuraUserWidget;
struct FGameplayTagContainer;
USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelSignature, float, NewValue,bool,bLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowDelegate,FUIWidgetRow,Row);
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FMessageWidgetRowDelegate MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|XP")
	FOnAttributeSignature OnXPChangedDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Level")
	FOnLevelSignature OnLevelChangedDelegate;
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag,const FGameplayTag& SlotTag,const FGameplayTag& StatusTag,const FGameplayTag& PreSlotTag);
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	template<typename  T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);
	
	void OnXPChanged(const int32 NewXP);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
