// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
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
	
	UPROPERTY()
	UButton* NextCameraButton = nullptr;
	UPROPERTY()
	UButton* PreviousCameraButton = nullptr;
	UPROPERTY()
	UButton* TriggerRespondersButton = nullptr;

	UPROPERTY()
	UImage* Screen = nullptr;

public:
	ACCTVTerminal* CCTVTerminal = nullptr;
};
