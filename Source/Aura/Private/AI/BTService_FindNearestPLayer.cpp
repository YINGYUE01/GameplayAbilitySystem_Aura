// Copyright YING


#include "AI/BTService_FindNearestPLayer.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPLayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(1,1.f,FColor::Red,*AIOwner->GetName());
	GEngine->AddOnScreenDebugMessage(2,1.f,FColor::Green,*ActorOwner->GetName());
	APawn* OwningPawn = AIOwner->GetPawn();
	const FName TargetTags = OwningPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");
	TArray<AActor*> ActorsWithTags;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn,TargetTags,ActorsWithTags);
	for (auto& Actor : ActorsWithTags)
	{
		
	}
}
