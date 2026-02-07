// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase,public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/* Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToLevel_Implementation(int32 InLevel) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;
	virtual int32 GetAttributePointsReward_Implementation(int32 CurrentLevel,int32 NewLevel) override;
	virtual int32 GetSpellPointsReward_Implementation(int32 CurrentLevel, int32 NewLevel) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	/* End Player Interface */
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/*End Combat Interface */

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastLevelUpParticles();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	
};
