// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilityInfo;
class AAuraPlayerState;
class AAuraPlayerController;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY();
	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS):
	PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS) { }
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController=nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState=nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent=nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet=nullptr;
	
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayStateChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,const FAuraAbilityInfo&,Info);
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValues();
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FAbilityInfoSignature AbilityInfoDelegate;
	
	void BroadcastAbilityInfo();
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
    	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	AAuraPlayerController* GetAuraPC();
	AAuraPlayerState* GetAuraPS();
	UAuraAbilitySystemComponent* GetAuraASC();
	UAuraAttributeSet* GetAuraAS();
	
};
