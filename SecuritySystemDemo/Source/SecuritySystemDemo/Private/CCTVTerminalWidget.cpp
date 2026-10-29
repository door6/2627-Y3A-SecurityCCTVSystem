// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTVTerminalWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "CCTVTerminal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTerminalWidget, Log, All);

TSharedRef<SWidget> UCCTVTerminalWidget::RebuildWidget()
{
	Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	WidgetTree->RootWidget = Cast<UWidget>(Root);

	if (!WidgetTree->RootWidget)
		UE_LOG(LogTerminalWidget, Error, TEXT("RootWidget is NULL"));

	NextCameraButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NextCameraButton"));
	NextCameraButton->SetBackgroundColor(FLinearColor::Blue);
	UCanvasPanelSlot* CanvasSlot = Root->AddChildToCanvas(NextCameraButton);
	CanvasSlot->SetPosition(FVector2D(1400.f, 600.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	PreviousCameraButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("PreviousCameraButton"));
	PreviousCameraButton->SetBackgroundColor(FLinearColor::Blue);
	CanvasSlot = Root->AddChildToCanvas(PreviousCameraButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 600.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	TriggerRespondersButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("TriggerRespondersButton"));
	TriggerRespondersButton->SetBackgroundColor(FLinearColor::Green);
	CanvasSlot = Root->AddChildToCanvas(TriggerRespondersButton);
	CanvasSlot->SetPosition(FVector2D(1400.f, 100.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	ExitButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ExitButton"));
	ExitButton->SetBackgroundColor(FLinearColor::Red);
	CanvasSlot = Root->AddChildToCanvas(ExitButton);
	CanvasSlot->SetPosition(FVector2D(100.f, 100.f));
	CanvasSlot->SetSize(FVector2D(50.f, 50.f));

	Screen = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

	UMaterialInterface* RenderMaterial = nullptr;
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget = nullptr;

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACCTVTerminal::StaticClass());
	if (!FoundActor)
	{
		UE_LOG(LogTerminalWidget, Warning, TEXT("NO CCTVTerminal in the scene"));

		RenderMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/SecurityCCTVSystem/M_Screen.M_Screen"));
		TextureRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1024, 1024, ETextureRenderTargetFormat::RTF_RGBA32f, FLinearColor::Black, false);
	}
	else
	{
		ACCTVTerminal* Terminal = Cast<ACCTVTerminal>(FoundActor);
		RenderMaterial = Terminal->RenderMaterial;
		TextureRenderTarget = Terminal->TextureRenderTarget;
	}

	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(RenderMaterial, this);
	DynamicMaterialInstance->SetTextureParameterValue(FName("ScreenTexture"), TextureRenderTarget);
	DynamicMaterialInstance->SetScalarParameterValue(FName("UVRotation"), 0.0f);
	Screen->SetBrushFromMaterial(DynamicMaterialInstance);

	CanvasSlot = Root->AddChildToCanvas(Screen);;
	CanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f)); // stretch full screen
	CanvasSlot->SetOffsets(FMargin(170.f, 90.0f, 170.f, 20.0f));

	return Super::RebuildWidget();
}

void UCCTVTerminalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindButtonsToOnClicked();
}

void UCCTVTerminalWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UnbindButtonsFromOnClicked();
}

void UCCTVTerminalWidget::BindButtonsToOnClicked()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCTVTerminal::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ACCTVTerminal* CCTVTerminal = Cast<ACCTVTerminal>(Actor);
		if (CCTVTerminal->IsInUse())
		{
			if (NextCameraButton)
				NextCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToNextCamera);
			if (PreviousCameraButton)
				PreviousCameraButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::SwitchToPreviousCamera);
			if (TriggerRespondersButton)
				TriggerRespondersButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TriggerResponders);
			if (ExitButton)
				ExitButton->OnClicked.AddDynamic(CCTVTerminal, &ACCTVTerminal::TurnOff);

			UE_LOG(LogTerminalWidget, Log, TEXT("Buttons bound to %s"), *CCTVTerminal->GetActorLabel());

			return;
		}			
	}
	UE_LOG(LogTerminalWidget, Warning, TEXT("No CCTV terminal is in use"));
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

	UE_LOG(LogTerminalWidget, Log, TEXT("Buttons removed"));
}

DEFINE_LOG_CATEGORY(LogTerminalWidget);