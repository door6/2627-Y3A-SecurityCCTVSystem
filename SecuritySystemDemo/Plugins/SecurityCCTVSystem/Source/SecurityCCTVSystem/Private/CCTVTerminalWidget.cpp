// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminalWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "CCTVTerminal.h"	// DONT move this to .h !!!
#include "Kismet/GameplayStatics.h"
#include "SecuritySystemLog.h"

TSharedRef<SWidget> UCCTVTerminalWidget::RebuildWidget()
{
	UE_LOG(LogSecuritySystem, Log, TEXT("CCTVTerminalWidget:NativeConstruct"));

	//UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	WidgetTree->RootWidget = Root;

	if (!WidgetTree->RootWidget)
		UE_LOG(LogSecuritySystem, Error, TEXT("RootWidget is NULL"));

	NextCameraButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NextCameraButton"));
	//NextCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToNextCamera);
	//NextCameraButton->SetBackgroundColor(FLinearColor::Red);
	//RootWidget->AddChild(NextCameraButton);
	NextCameraButton->SetBackgroundColor(FLinearColor::Blue);
	UCanvasPanelSlot* CanvasSlot = Root->AddChildToCanvas(NextCameraButton);
	CanvasSlot->SetPosition(FVector2D(1400.f, 600.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	PreviousCameraButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("PreviousCameraButton"));
	//PreviousCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToPreviousCamera);
	//RootWidget->AddChild(PreviousCameraButton);
	PreviousCameraButton->SetBackgroundColor(FLinearColor::Blue);
	CanvasSlot = Root->AddChildToCanvas(PreviousCameraButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 600.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	TriggerRespondersButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("TriggerRespondersButton"));
	//TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
	//RootWidget->AddChild(TriggerRespondersButton);
	TriggerRespondersButton->SetBackgroundColor(FLinearColor::Green);
	CanvasSlot = Root->AddChildToCanvas(TriggerRespondersButton);
	CanvasSlot->SetPosition(FVector2D(1400.f, 100.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	ExitButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ExitButton"));
	//TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
	//RootWidget->AddChild(TriggerRespondersButton);
	ExitButton->SetBackgroundColor(FLinearColor::Red);
	CanvasSlot = Root->AddChildToCanvas(ExitButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 100.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	Screen = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	//Screen->SetDesiredSizeOverride(FVector2D(2.0f, 3.0f));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCTVTerminal::StaticClass(), FoundActors);
	//CCTVTerminal = Cast<ACCTVTerminal>(FoundActors[0]);
	ACCTVTerminal* Terminal = Cast<ACCTVTerminal>(FoundActors[0]);

	//UMaterialInterface* Material = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/SecurityCCTVSystem/M_Screen.M_Screen")).Object;
	// 
	//Screen->SetBrushFromMaterial(CCTVTerminal->DynamicMaterialInstance);		//Material

	/*Screen->SetBrushResourceObject(CCTVTerminal->RenderMaterial);		//TextureRenderTarget
	if (!CCTVTerminal->RenderMaterial)
		UE_LOG(LogSecuritySystem, Error, TEXT("CCTVTerminal->RenderMaterial is NULL"));
	//Screen->SetBrushFromTexture(CCTVTerminal->TextureRenderTarget);
	Screen->SetDesiredSizeOverride(FVector2D(600.f, 400.f));*/

	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Terminal->RenderMaterial, this);
	DynamicMaterialInstance->SetTextureParameterValue(FName("ScreenTexture"), Terminal->TextureRenderTarget);
	DynamicMaterialInstance->SetScalarParameterValue(FName("UVRotation"), 0.0f);
	Screen->SetBrushFromMaterial(DynamicMaterialInstance);

	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/SecurityCCTVSystem/M_Screen.M_Screen"));
	if (MaterialFinder.Succeeded())
	{
		Screen->SetBrushFromMaterial(MaterialFinder.Object);
	}*/

	//DebugBG->SetColorAndOpacity(FLinearColor::Red);
	CanvasSlot = Root->AddChildToCanvas(Screen);
	//CanvasSlot->SetPosition(FVector2D(300.f, 400.f));
	//CanvasSlot->SetSize(FVector2D(600.f, 400.f));
	CanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f)); // stretch full screen
	CanvasSlot->SetOffsets(FMargin(170.f, 90.0f, 170.f, 20.0f));

	return Super::RebuildWidget();
}

void UCCTVTerminalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind delegates here
	BindButtonsToOnClicked();
	/*if (NextCameraButton)
		NextCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToNextCamera);
	if (PreviousCameraButton)
		PreviousCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToPreviousCamera);
	if (TriggerRespondersButton)
		TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TurnOff);*/
}

void UCCTVTerminalWidget::BindButtonsToOnClicked()
{
	if (NextCameraButton)
		NextCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToNextCamera);
	if (PreviousCameraButton)
		PreviousCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToPreviousCamera);
	if (TriggerRespondersButton)
		TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TurnOff);

	UE_LOG(LogSecuritySystem, Log, TEXT("Buttons bound"));
}

void UCCTVTerminalWidget::UnbindButtonsFromOnClicked()
{
	if (NextCameraButton)
		NextCameraButton->OnClicked.Clear();
	if (PreviousCameraButton)
		PreviousCameraButton->OnClicked.Clear();
	if (TriggerRespondersButton)
		TriggerRespondersButton->OnClicked.Clear();
	if (ExitButton)
		ExitButton->OnClicked.Clear();

	UE_LOG(LogSecuritySystem, Log, TEXT("Buttons removed"));
}

