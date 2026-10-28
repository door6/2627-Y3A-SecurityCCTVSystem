// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "CCTVTerminalWidget.generated.h"

//class ACCTVTerminal;

/**
 * 
 */
UCLASS()
class UCCTVTerminalWidget : public UUserWidget		// SECURITYCCTVSYSTEM_API
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	//UCanvasPanel* Root = nullptr;
	TObjectPtr<UCanvasPanel> Root;
	//UWidget* Root = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	//UButton* NextCameraButton = nullptr;
	TObjectPtr<UButton> NextCameraButton;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	//UButton* PreviousCameraButton = nullptr;
	TObjectPtr<UButton> PreviousCameraButton;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	//UButton* TriggerRespondersButton = nullptr;
	TObjectPtr<UButton> TriggerRespondersButton;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	//UButton* ExitButton = nullptr;
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	//UImage* Screen = nullptr;
	TObjectPtr<UImage> Screen;

	void BindButtonsToOnClicked();
	void UnbindButtonsFromOnClicked();

	//ACCTVTerminal* CCTVTerminal = nullptr;
};
