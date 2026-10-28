// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTVInteractInputAction.h"
#include "CCTVInputMappingContext.h"

#include "InputManager.generated.h"

UCLASS()
class AInputManager : public AActor
{
	GENERATED_BODY()

public:

	AInputManager();

	virtual void BeginPlay() override;

	void AccessCCTVTerminal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UCCTVInteractInputAction> InteractInputAction;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintType, Category = Input)		//"Security System"
	TObjectPtr<UCCTVInputMappingContext> InputMapping;
};
