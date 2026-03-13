// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;



};
