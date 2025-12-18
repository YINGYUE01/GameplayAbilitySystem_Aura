// Copyright YING


#include "AbilitySystem/Ability/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread/NumMinions;
	TArray<FVector> SpawnLocations;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread/2,FVector::UpVector);
	for (int32 i = 0;i<NumMinions;i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
		FVector ChosenLocation = Location + Direction*FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,ChosenLocation+FVector(0.f,0.f,400.f),ChosenLocation-FVector(0.f,0.f,400.f),ECollisionChannel::ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenLocation=Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	int32 Rand = FMath::RandRange(0,MinionClasses.Num()-1);
	return MinionClasses[Rand];
}
