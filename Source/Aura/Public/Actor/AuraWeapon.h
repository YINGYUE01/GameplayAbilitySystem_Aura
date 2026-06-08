// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraWeapon.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
UCLASS()
class AURA_API AAuraWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraWeapon();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="WeaponMesh")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CollisionBox")
	TObjectPtr<UBoxComponent> Box;
	
protected:
	virtual void BeginPlay() override;
	
};
