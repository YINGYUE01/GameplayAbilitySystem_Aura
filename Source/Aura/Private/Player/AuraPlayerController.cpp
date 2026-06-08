// Copyright YING


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Actor/MagicCircle.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacter.h"
#include "Character/AuraCharacterBase.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/HighLightInterface.h"
#include "UI/Widget/DamageTextComponent.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
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

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
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
		if (DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutonRunning = false;
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (IsValid(ThisActor) && ThisActor->Implements<UHighLightInterface>())
			IHighLightInterface::Execute_UnHighlightActor(ThisActor);
		if (IsValid(LastActor) && LastActor->Implements<UHighLightInterface>())
			IHighLightInterface::Execute_UnHighlightActor(LastActor);
		ThisActor = nullptr;
		LastActor = nullptr;
		return;
	}
	const ECollisionChannel Channel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(Channel,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighLightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}
	if (LastActor != ThisActor)
	{
		if (IsValid(ThisActor) && ThisActor->Implements<UHighLightInterface>())
			IHighLightInterface::Execute_HighlightActor(ThisActor);
		if (IsValid(LastActor) && LastActor->Implements<UHighLightInterface>())
			IHighLightInterface::Execute_UnHighlightActor(LastActor);
	}
}

void AAuraPlayerController::AbilityInputTagPresses(FGameplayTag InputTag)
{
	//如果被施加Player_Block_InputPressed标签则不能激活技能
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		 return;
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNotEnemy;
			bAutonRunning = false;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
	}
	//根据输入Tag转入ASC激活技能
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
		return;
	/*  若不是鼠标左键  直接激活技能*/
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())  GetASC()->AbilityInputTagReleased(InputTag);
		bAutonRunning = false;
		return;
	}

	if (GetASC())  GetASC()->AbilityInputTagReleased(InputTag);
	//左键短按判断   若无目标且shift没有被按住 则判定为自动寻路，处理寻路逻辑
	if (TargetingStatus!=ETargetingStatus::TargetingEnemy && !bShiftPressed)
	{
		APawn* ControllerPawn = GetPawn();
		if (FollowTime<=ShortPressThreshold && ControllerPawn)
		{
			if (IsValid(ThisActor) && ThisActor->Implements<UHighLightInterface>())
			{
				IHighLightInterface::Execute_SetMoveToLocation(ThisActor,CachedDestination);
			}
			else
			{
				if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ClickNiagaraSystem,CachedDestination);
				}
			}
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControllerPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& NavLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(NavLoc,ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num()>=1) CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num()-1];
				bAutonRunning = true;
			}
		}
		TargetingStatus = ETargetingStatus::NotTargeting;
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
		return;
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		bAutonRunning = false;
		return;
	}
	//鼠标左键
	if (TargetingStatus==ETargetingStatus::TargetingEnemy || bShiftPressed )
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)  CachedDestination = CursorHit.ImpactPoint;
		if (APawn* ControllerPawn = GetPawn())
		{
			bAutonRunning  = false;
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
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

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* Material)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (Material!=nullptr)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0,Material);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();	
	}
}

void AAuraPlayerController::SetMagicCircleMaterial(UMaterialInterface* Material)
{
	if (IsValid(MagicCircle))
	{
		if (Material!=nullptr)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0,Material);
		}
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount,ACharacter* TargetCharacter,bool bBlockHit,bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount,bBlockHit,bCriticalHit);
	}
}



void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		return;
	bAutonRunning = false;
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}

void AAuraPlayerController::Steering(const struct FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		return;
	if (GetCharacter()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_Steering(GetCharacter(),InputActionValue.Get<float>());
	}
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::ShiftPress);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftRelease);
	AuraInputComponent->BindAction(ForwardAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Steering);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&AAuraPlayerController::AbilityInputTagPresses,&AAuraPlayerController::AbilityInputTagReleased,&AAuraPlayerController::AbilityInputTagHeld);
}
