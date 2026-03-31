// Copyright YING


#include "CheckPoint/MapEntrance.h"

#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void AMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{
	
}

void AMapEntrance::OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			AuraGM->SaveWorldState(GetWorld(),DestinationWorld.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveGameProgress(OtherActor,DestinationWorldPlayerStartTag);
		UGameplayStatics::OpenLevelBySoftObjectPtr(this,DestinationWorld);
	}
}

void AMapEntrance::BeginPlay()
{
	Super::BeginPlay();
}
