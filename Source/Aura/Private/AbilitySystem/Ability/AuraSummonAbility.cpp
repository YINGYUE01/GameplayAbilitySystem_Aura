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
		const FVector ChosenLocation = Location + Direction*FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
		DrawDebugSphere(GetWorld(),ChosenLocation,25.f,12,FColor::Red,false,3.f);
		SpawnLocations.Add(ChosenLocation);
		DrawDebugSphere(GetWorld(),Location + Direction*MinSpawnDistance,10.f,12,FColor::Red,false,3.f);
		DrawDebugSphere(GetWorld(),Location+Direction*MaxSpawnDistance,10.f,12,FColor::Red,false,3.f);
	}
	return SpawnLocations;
}
