// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminalWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "CCTVTerminal.h"	// DONT move this to .h !!!
#include "SecuritySystemLog.h"

TSharedRef<SWidget> UCCTVTerminalWidget::RebuildWidget()
{
	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminalWidget:NativeConstruct"));

	//UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	UCanvasPanel* Root = WidgetTree.Get()->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	WidgetTree->RootWidget = Root;

	//temp for debugging
	/*UImage* DebugBG = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	DebugBG->SetColorAndOpacity(FLinearColor::Red);
	UCanvasPanelSlot* BGSlot = Root->AddChildToCanvas(DebugBG);
	BGSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f)); // stretch full screen
	BGSlot->SetOffsets(FMargin(0.f));*/

	if (!WidgetTree->RootWidget)
		UE_LOG(LogSecuritySystem, Error, TEXT("RootWidget is NULL"));

	NextCameraButton = WidgetTree.Get()->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NextCameraButton"));
	//NextCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToNextCamera);
	//NextCameraButton->SetBackgroundColor(FLinearColor::Red);
	//RootWidget->AddChild(NextCameraButton);
	UCanvasPanelSlot* CanvasSlot = Root->AddChildToCanvas(NextCameraButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 100.f));
	CanvasSlot->SetSize(FVector2D(200.f, 50.f));

	PreviousCameraButton = WidgetTree.Get()->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("PreviousCameraButton"));
	//PreviousCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToPreviousCamera);
	//RootWidget->AddChild(PreviousCameraButton);
	CanvasSlot = Root->AddChildToCanvas(PreviousCameraButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 200.f));
	CanvasSlot->SetSize(FVector2D(200.f, 50.f));

	TriggerRespondersButton = WidgetTree.Get()->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("TriggerRespondersButton"));
	//TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
	//RootWidget->AddChild(TriggerRespondersButton);
	CanvasSlot = Root->AddChildToCanvas(TriggerRespondersButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 300.f));
	CanvasSlot->SetSize(FVector2D(200.f, 50.f));

	return Super::RebuildWidget();
}

void UCCTVTerminalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind delegates here

	if (NextCameraButton)
		NextCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToNextCamera);
	if (PreviousCameraButton)
		PreviousCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToPreviousCamera);
	if (TriggerRespondersButton)
		TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
}

