// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	void OnHit();
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UFUNCTION()
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlapedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult );


	bool bHit = false;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
	float SpanLife = 15.f;

	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComponent;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere,Category="FireSound")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr< UAudioComponent> AudioComponent;
	
};
