// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Debuff")
	FGameplayTag DebuffTag;
	UDebuffNiagaraComponent();
protected:

	virtual void BeginPlay() override;
	void DebuffTagChanged(const FGameplayTag CallbackTag,int32 NewCount);
	
	UFUNCTION()
	void OnOwnerDeath(AActor* Actor);
};
