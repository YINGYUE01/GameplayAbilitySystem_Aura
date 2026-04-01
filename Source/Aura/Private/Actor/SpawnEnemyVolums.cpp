// Copyright YING


#include "Actor/SpawnEnemyVolums.h"

#include "Actor/SpawnEnemyPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

ASpawnEnemyVolums::ASpawnEnemyVolums()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Overlap);
}

void ASpawnEnemyVolums::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

void ASpawnEnemyVolums::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ASpawnEnemyVolums::OnBoxOverlay);
}

void ASpawnEnemyVolums::OnBoxOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	bReached = true;
	for (auto& Enemy : EnemyPoints)
	{
		if (IsValid(Enemy))
			Enemy->SpawnEnemy();
	}
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
