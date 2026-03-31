// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
public:

	virtual void HighlightActor_Implementation() override;
	/*SaveInterface*/
	virtual void LoadActor_Implementation() override;

	UPROPERTY(EditAnywhere,Category="Map")
	TSoftObjectPtr<UWorld> DestinationWorld;

	UPROPERTY(EditAnywhere,Category="Map")
	FName DestinationWorldPlayerStartTag;

	virtual void OnSphereOverlay(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult ) override;

protected:
	virtual void BeginPlay() override;
};
