// Copyright YING
#include "CheckPoint/CheckPoint.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Interfaces/IPluginManager.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckPointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Overlap);
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ACheckPoint::OnSphereOverlay);
}

void ACheckPoint::OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			AuraGM->SaveWorldState(GetWorld());
		}
		
		HandleGlowEffects();
		IPlayerInterface::Execute_SaveGameProgress(OtherActor,PlayerStartTag);
	}
}

void ACheckPoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0),this);
	CheckPointMesh->SetMaterial(0,MaterialInstanceDynamic);
	CheckPointReached(MaterialInstanceDynamic);
}
