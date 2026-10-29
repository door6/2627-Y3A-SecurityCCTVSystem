// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "CCTVTerminalWidget.generated.h"

UCLASS()
class UCCTVTerminalWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> Root;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UButton> NextCameraButton;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UButton> PreviousCameraButton;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UButton> TriggerRespondersButton;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UImage> Screen;

	void BindButtonsToOnClicked();
	void UnbindButtonsFromOnClicked();
};
