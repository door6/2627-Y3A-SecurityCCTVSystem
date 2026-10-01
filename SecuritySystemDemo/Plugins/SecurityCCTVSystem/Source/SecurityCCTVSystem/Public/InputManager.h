// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/Object.h"
#include "GameFramework/Actor.h"
//#include "InputAction.h"
#include "CCTVInteractInputAction.h"
#include "CCTVInputMappingContext.h"

#include "InputManager.generated.h"

/**
 * 
 */
UCLASS()
class SECURITYCCTVSYSTEM_API AInputManager : public AActor
{
	GENERATED_BODY()

public:

	AInputManager();

	virtual void BeginPlay() override;

	void AccessCCTVTerminal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UCCTVInteractInputAction* InteractInputAction;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintType, Category = "Security System")
	UCCTVInputMappingContext* InputMapping;
	//TObjectPtr<UCCTVInputMappingContext> InputMapping;

	//temp for testing
	UPROPERTY(EditAnywhere)
	FKey MyKey = EKeys::E;
	
};
