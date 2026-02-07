// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	//~begin EnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~end EnemyInterface
	UPROPERTY(BlueprintReadOnly)
	bool bHighlight = false;
	virtual AActor* GetCombatTarget_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	/*
	 *  Combatinterface
	 */
	virtual int32 GetPlayerLevel_Implementation() override;

	virtual void Die() override;
	/*
	* end Combatinterface
	*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SpanLife = 5.f;
		
	UPROPERTY(BlueprintAssignable)
	FOnAttributeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeSignature OnMaxHealthChanged;

	virtual void PossessedBy(AController* NewController) override;

	void BindUI();

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> CombatTarget;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;


	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	void HitReactTagChanged(const FGameplayTag CallBackTag,int32 NewCount);
	/* AI */
	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
