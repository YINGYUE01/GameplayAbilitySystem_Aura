// Copyright YING


#include "Actor/AuraWeapon.h"

#include "Components/BoxComponent.h"

AAuraWeapon::AAuraWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetupAttachment(GetRootComponent());
}

void AAuraWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}