// Copyright YING

#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	Pt_0 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_0"));
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);
	Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_1"));
	ImmutablePts.Add(Pt_1);
	Pt_1->SetupAttachment(Pt_0);
	Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_2"));
	ImmutablePts.Add(Pt_2);
	Pt_2->SetupAttachment(Pt_0);
	Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_3"));
	ImmutablePts.Add(Pt_3);
	Pt_3->SetupAttachment(Pt_0);
	Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_4"));
	ImmutablePts.Add(Pt_4);
	Pt_4->SetupAttachment(Pt_0);
	Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_5"));
	ImmutablePts.Add(Pt_5);
	Pt_5->SetupAttachment(Pt_0);
	Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_6"));
	ImmutablePts.Add(Pt_6);
	Pt_6->SetupAttachment(Pt_0);
	Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_7"));
	ImmutablePts.Add(Pt_7);
	Pt_7->SetupAttachment(Pt_0);
	Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_8"));
	ImmutablePts.Add(Pt_8);
	Pt_8->SetupAttachment(Pt_0);
	Pt_9 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_9"));
	ImmutablePts.Add(Pt_9);
	Pt_9->SetupAttachment(Pt_0);
	Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_10"));
	ImmutablePts.Add(Pt_10);
	Pt_10->SetupAttachment(Pt_0);
	
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}


TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,float YawOverride)
{
	checkf(ImmutablePts.Num()>=NumPoints,TEXT("Attempted to access ImmutablePts out of bounds"));
	TArray<USceneComponent*> ArrayCopy;
	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num()>NumPoints) return ArrayCopy;
		if (Pt!=Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation()-Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride,FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation()+ToPoint);
		}
		//光线追踪
		const FVector RaisedLocation = FVector(Pt_0->GetComponentLocation().X,Pt_0->GetComponentLocation().Y,Pt_0->GetComponentLocation().Z+500.f);
		const FVector LoweredLocation = FVector(Pt_0->GetComponentLocation().X,Pt_0->GetComponentLocation().Y,Pt_0->GetComponentLocation().Z-500.f);
		TArray<AActor*> IgnoreActors;
		FHitResult HitResult;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this,IgnoreActors,TArray<AActor*>(),1500.f,GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult,RaisedLocation,LoweredLocation,FName("BlockAll"),QueryParams);
		Pt->SetWorldLocation(FVector(Pt->GetComponentLocation().X,Pt->GetComponentLocation().Y,HitResult.ImpactPoint.Z));
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}
