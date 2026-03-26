// Copyright YING


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "Tools/UEdMode.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomComponent");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
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
	//Sever InitS AbilitySystemInfo
	/**
	 * InitAbilityActorInfo() 初始化Character的AbilitySystemComponent、AttributeSet
	 */
	InitAbilityActorInfo();
	LoadData();
	/*
	添加角色的基础技能 在基类中已经实现 而基础技能是在基类声明的StartupAbilities
	AddCharacterAbilities（） 函数会呼叫 ASC->GiveAbility（） 来赋予技能。
	AddCharacterAbilities依赖于Character的AbilitySystemComponent
	由于赋予技能是由ASC完成的，所以这一步是在InitAbilityActorInfo（）执行之后的因为在里面PlayerState将真正搭载的ASC和AS赋予Character
	*/
}

void AAuraCharacter::LoadData()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameModeBase)
	{
		ULoadScreenSaveGame* SaveData = AuraGameModeBase->RetrieveInGameSaveData();
		if (SaveData==nullptr) return;
		if (SaveData->bFirstSave)
		{
			AddCharacterAbilities();
		}
		else
		{
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			UAuraAbilitySystemLibrary::InitializeDefaultAttributeWithSaveGame(this,AbilitySystemComponent,SaveData);
		}
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Client Init AbilitySystemInfo
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_Stuned()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockTags;
		BlockTags.AddTag(FAuraGameplayTags::Get().Player_Block_CursorTrace);
		BlockTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputHeld);
		BlockTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputPressed);
		BlockTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputReleased);
		if (Stunned)
		{
			AuraASC->AddLooseGameplayTags(BlockTags);
			StunDebuffNiagaraComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockTags);
			StunDebuffNiagaraComponent->Deactivate();
		}
		
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (Burned)
	{
		BurnDebuffNiagaraComponent->Activate();
	}
	else
	{
		BurnDebuffNiagaraComponent->Deactivate();
	}
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::AddToLevel_Implementation(int32 InLevel)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InLevel);
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
	
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation()
{
	if (LevelUpNiagaraComponent)
	{
		FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		FVector NiagaraComponentLocation = TopDownCameraComponent->GetComponentLocation();
		FRotator DirRotation = (NiagaraComponentLocation - CameraLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(DirRotation);
		LevelUpNiagaraComponent->Activate();
	}
		
}



int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 CurrentLevel,int32 NewLevel)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	int32 AttributePointsReward = 0;
	for (int i = CurrentLevel;i<NewLevel;i++)
	{
		AttributePointsReward += AuraPlayerState->LevelUpInfo->LevelUpInformation[i].AttributePointAward;
	}
	return AttributePointsReward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 CurrentLevel, int32 NewLevel)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	int32 SpellPointsReward = 0;
	for (int i = CurrentLevel;i<NewLevel;i++)
	{
		SpellPointsReward += AuraPlayerState->LevelUpInfo->LevelUpInformation[i].SpellPointAward;
	}
	return SpellPointsReward;
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* Material)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(Material);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SetMagicCircleMaterial_Implementation(UMaterialInterface* Material)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->SetMagicCircleMaterial(Material);
	}
}

void AAuraCharacter::SaveGameProgress_Implementation(const FName& CheckPointTag)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameModeBase)
	{
		ULoadScreenSaveGame* SaveData = AuraGameModeBase->RetrieveInGameSaveData();
		if (SaveData==nullptr) return;
		SaveData->PlayerStartTag = CheckPointTag;
		AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
		if (AuraPlayerState)
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}
		SaveData->bFirstSave = false;
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		if (!HasAuthority()) return;
		FForEachAbility ForEachAbilityDelegate;
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		ForEachAbilityDelegate.BindLambda([this,&AuraASC,&SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
			FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			FSaveAbility SaveAbility;
			SaveAbility.AbilityTag = AbilityTag;
			SaveAbility.AbilityLevel = AbilitySpec.Level;
			SaveAbility.AbilityClass = Info.Ability;
			SaveAbility.AbilitySlot = AuraASC->GetSlotTagFromAbilityTag(AbilityTag);
			SaveAbility.AbilityStatus = AuraASC->GetStatusTagFromAbilityTag(AbilityTag);
			SaveAbility.AbilityType = Info.AbilityType;
			SaveData->SaveAbilities.Add(SaveAbility);
		});
		AuraASC->ForEachAbility(ForEachAbilityDelegate);
		AuraGameModeBase->SaveInGameProgressData(SaveData);
	}
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
	OnAscRegister.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AAuraCharacter::StunTagChanged);
	
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

