// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintImplementableEvent)
	void StatOutgoingTimeline();
	virtual void OnHit() override;

	UPROPERTY(BlueprintReadOnly,Replicated)
	TObjectPtr<AActor> ReturnToActor;
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplodeDamageEffectParams;
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlapedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult ) override;

	
};
