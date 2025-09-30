// Copyright YING


#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController;
	
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass UnInitialized,please fill out BP_Overlay"));
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass UnInitialized,please fill out BP_AuraHUD"));
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);
	Widget->AddToViewport();
}
