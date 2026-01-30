// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowDelegate,FUIWidgetRow,Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,const FAuraAbilityInfo&,Info);

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
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|XP")
	FOnAttributeSignature OnXPChangedDelegate;
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;
	template<typename  T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);

	void OnInitialStartUpAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent);
	
	void OnXPChanged(int32 NewXP);
};



template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
