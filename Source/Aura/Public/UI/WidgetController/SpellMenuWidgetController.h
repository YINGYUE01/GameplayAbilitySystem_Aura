// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

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
	
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;
};
