// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "CCTVTerminalWidget.generated.h"

class ACCTVTerminal;

/**
 * 
 */
UCLASS()
class SECURITYCCTVSYSTEM_API UCCTVTerminalWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UCanvasPanel* Root = nullptr;
	//UWidget* Root = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* NextCameraButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* PreviousCameraButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* TriggerRespondersButton = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ExitButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UImage* Screen = nullptr;

public:
	void BindButtonsToOnClicked();
	void UnbindButtonsFromOnClicked();

	ACCTVTerminal* CCTVTerminal = nullptr;
};
