// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"
class ULevelUpInfos;
class UAttributeSet;
class UAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,int32 /*StateValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChanged,int32,bool);
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelUpInfos> LevelUpInfo;
	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnPlayerLevelChanged OnLevelChangedDelegate;
	FOnPlayerStateChanged OnAttributePointsChangedDelegate;
	FOnPlayerStateChanged OnSpellPointsChangedDelegate;
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; } 
	FORCEINLINE int32 GetXP() const { return XP; } 
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	void SetAttributePoints(int32 NewAttributePoints);
	void SetSpellPoints(int32 NewSpellPoints);
	
	void AddToXP(int32 Amount);
	void AddToLevel(int32 Amount);
	void AddToAttributePoints(int32 Amount);
	void AddToSpellPoints(int32 Amount);
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>	AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_XP)
	int32 XP = 0;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
};
