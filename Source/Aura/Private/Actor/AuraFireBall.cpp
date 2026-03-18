// Copyright YING


#include "Actor/AuraFireBall.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StatOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlay(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
