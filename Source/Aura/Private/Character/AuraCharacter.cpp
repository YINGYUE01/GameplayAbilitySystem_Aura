// Copyright YING


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//Sever InitS AbilitySysetemInfo
	/**
	 * InitAbilityActorInfo() 初始化Character的AbilitySystemComponent、AttributeSet
	 */
	InitAbilityActorInfo();
	/*
	添加角色的基础技能 在基类中已经实现 而基础技能是在基类声明的StartupAbilities
	AddCharacterAbilities（） 函数会呼叫 ASC->GiveAbility（） 来赋予技能。
	AddCharacterAbilities依赖于Character的AbilitySystemComponent
	由于赋予技能是由ASC完成的，所以这一步是在InitAbilityActorInfo（）执行之后的因为在里面PlayerState将真正搭载的ASC和AS赋予Character
	*/
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Client Init AbilitySystemInfo
	InitAbilityActorInfo();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	//设置“OwnerActor 逻辑拥有者”和“AvatarActor 物理执行者”
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	//将搭载在PlayerState上的ASC和AS赋予角色
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	//初始化初始属性
	/**
	角色需要执行 基类就实现的
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);  初始化主要属性
	ApplyEffectToSelf(DefaultSecondaryAttributes,1);  初始化次要属性
	ApplyEffectToSelf(DefaultVitalAttributes,1);  初始化首要属性
	敌人需要执行 不执行基类实现的逻辑
	其单独的属性，这些属性由敌人的类别决定例如：
	战士执行战士属性
	射手执行射手属性
	 */
	InitializeDefaultAttributes();
	
	//将AbilityActorInfo已设置的事件广播出去
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			//初始化HUD的信息AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet以便实现数据驱动
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}

