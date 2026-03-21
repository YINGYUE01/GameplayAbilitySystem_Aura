// Copyright YING


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/LoadScreenWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();
	LoadScreenViewMode = NewObject<UMVVM_LoadScreen>(this,LoadScreenViewModeClass);
	LoadScreenViewMode->InitializedLoadSLot();
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(),LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();
}
