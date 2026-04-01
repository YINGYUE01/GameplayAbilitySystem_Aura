// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "SpawnEnemyVolums.generated.h"

class ASpawnEnemyPoint;
class UBoxComponent;

UCLASS()
class AURA_API ASpawnEnemyVolums : public AActor,public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ASpawnEnemyVolums();
	/*  SaveInterface  */
	virtual void LoadActor_Implementation() override;
	/*  SaveInterface  */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(EditAnywhere)
	TArray<ASpawnEnemyPoint*> EnemyPoints;

	UPROPERTY(EditAnywhere,SaveGame)
	bool bReached = false;
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnBoxOverlay(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult );

};
