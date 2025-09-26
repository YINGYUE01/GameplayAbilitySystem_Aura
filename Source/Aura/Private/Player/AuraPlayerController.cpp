// Copyright YING


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	/*
	 * 多种情况说明:
	 * 1.LastActor 和 ThisActor 都为nullptr:
	 *		什么的不做
	 * 2.LastActor == nullptr && ThisActor有效:
	 *		ThisActor->HighLight()
	 * 3.LastAcotr有效ThisActor==nullptr：
	 *		LastActor->UnHighLight()
	 * 4.LastActor 和 ThisActor 都有效但不相等时:
	 *		LastActor->UnHighLight()
	 *		ThisActor->HighLight()
	 * 5.LastActor 和 ThisActor 都有效但相等时:
	 *		什么都不做
	 */
	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
			ThisActor->HighlightActor();
	}
	else
	{
		if (ThisActor == nullptr)
			LastActor->UnHighlightActor();
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);

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
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	
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

