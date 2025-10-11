// Copyright YING


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}



void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutonRunning) return;
	if (APawn* Controller = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(Controller->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		Controller->AddMovementInput(Direction,1.f);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination<=AutoRunAcceptanceadius)
		{
			bAutonRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (ThisActor) ThisActor->HighlightActor();
		if (LastActor) LastActor->UnHighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPresses(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*InputTag.ToString());
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutonRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		APawn* ControllerPawn = GetPawn();
		if (FollowTime<=ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControllerPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& NavLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(NavLoc,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),NavLoc,10.f,8.f,FColor::Silver,false,5.f);
				}
				if (NavPath->PathPoints.Num()>=1)
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num()-1];
				bAutonRunning = true;
			}
		}
		bTargeting = false;
		FollowTime = 0.f;
	}
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDrection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDrection);
		}
	}

}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraASC==nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraASC;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&AAuraPlayerController::AbilityInputTagPresses,&AAuraPlayerController::AbilityInputTagReleased,&AAuraPlayerController::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotaion(0.f,Rotation.Yaw,0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

