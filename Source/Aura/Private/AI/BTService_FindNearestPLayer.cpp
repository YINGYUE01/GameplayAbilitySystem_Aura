// Copyright YING


#include "AI/BTService_FindNearestPLayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPLayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* OwningPawn = AIOwner->GetPawn();
	const FName TargetTags = OwningPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");
	TArray<AActor*> ActorsWithTags;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn,TargetTags,ActorsWithTags);
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (auto Actor : ActorsWithTags)
	{
		if (IsValid(Actor) && IsValid(OwningPawn) && !ICombatInterface::Execute_bIsDead(Actor))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance<ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector,ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToTargetSelector,ClosestDistance);
}
