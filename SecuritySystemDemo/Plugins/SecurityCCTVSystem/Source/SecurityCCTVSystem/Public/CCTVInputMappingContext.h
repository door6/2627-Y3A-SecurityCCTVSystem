// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "CCTVInteractInputAction.h"
#include "CCTVInputMappingContext.generated.h"

/**
 * 
 */
UCLASS(/*ClassGroup = (Custom), meta = (EditAnywhere, Blueprintable, BlueprintType)*/)
class SECURITYCCTVSYSTEM_API UCCTVInputMappingContext : public UInputMappingContext
{
	GENERATED_BODY()
	
private:
	//UPROPERTY(EditAnywhere, Category = "Security System")
	/*UCCTVInteractInputAction* CCTVInteractInputAction;
	UCCTVNextCamInputAction* CCTVNextCamInputAction;
	UCCTVPrevCamInputAction* CCTVPrevCamInputAction;
	UCCTVTriggerInputAction* CCTVTriggerInputAction;
	UCCTVExitInputAction* CCTVExitInputAction;*/

public:
	//UPROPERTY(EditAnywhere, Category = "Security System")
	FEnhancedActionKeyMapping InteractMapping;
	//int InteractKeyIndex;

	UCCTVInputMappingContext();

	FEnhancedActionKeyMapping& GetInteractKeyMapping();

	void PostInitProperties();

};
