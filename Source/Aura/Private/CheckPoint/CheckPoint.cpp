// Copyright YING
#include "CheckPoint/CheckPoint.h"

#include "Aura/Aura.h"
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

	CheckPointMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	CheckPointMesh->MarkRenderStateDirty();
	SceneComponent=CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SceneComponent->SetupAttachment(GetRootComponent());

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckPointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Overlap);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	if (bBindOverlapCallback)
		Sphere->OnComponentBeginOverlap.AddDynamic(this,&ACheckPoint::OnSphereOverlay);
}

void ACheckPoint::SetMoveToLocation_Implementation(FVector& OutLocation)
{
	OutLocation = SceneComponent->GetComponentLocation();
}

void ACheckPoint::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void ACheckPoint::UnHighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(false);
}

void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
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
