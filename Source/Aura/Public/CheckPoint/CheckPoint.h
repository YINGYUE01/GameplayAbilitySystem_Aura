// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighLightInterface.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACheckPoint : public APlayerStart , public ISaveInterface,public IHighLightInterface
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(BlueprintReadOnly,SaveGame)
	bool bReached = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	/* Highlight Interface */
	void SetMoveToLocation_Implementation(FVector& OutLocation) override;
	void HighlightActor_Implementation() override;
	void UnHighlightActor_Implementation() override;

	/*SaveInterface*/
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult );

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	
};
