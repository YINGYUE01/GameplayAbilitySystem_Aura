// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraPlayerController.generated.h"

class AMagicCircle;
class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	
	//鼠标驱动的移动
	void AutoRun();
	virtual void PlayerTick(float DeltaTime) override;
	//
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;
	UAuraAbilitySystemComponent* GetASC();
	//显示伤害数字函数
	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float DamageAmount,ACharacter* TargetCharacter,bool bBlockHit,bool bCriticalHit);
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* Material);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	UFUNCTION(BlueprintCallable)
	void SetMagicCircleMaterial(UMaterialInterface* Material);

	void UpdateMagicCircleLocation();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	//键盘移动函数
	void Move(const struct FInputActionValue& InputActionValue);
	//鼠标光标检测
	void CursorTrace();
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	void ShiftPress() { bShiftPressed = true;} ;
	void ShiftRelease() { bShiftPressed=false; } ;
	bool bShiftPressed = false;
	
	FHitResult CursorHit;
	//高光描边显示转换上下Actor
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	void AbilityInputTagPresses(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//Action与InputTag绑定的集合
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	FVector CachedDestination;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutonRunning=false;
	bool bTargeting  = false;
	UPROPERTY(EditAnywhere)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
};
