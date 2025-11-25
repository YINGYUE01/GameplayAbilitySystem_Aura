// Copyright YING


#include "AI/BTService_FindNearestPLayer.h"

#include "AIController.h"

void UBTService_FindNearestPLayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(1,1.f,FColor::Red,*AIOwner->GetName());
	GEngine->AddOnScreenDebugMessage(2,1.f,FColor::Green,*ActorOwner->GetName());
}
