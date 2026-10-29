// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "CCTVInteractInputAction.h"
#include "CCTVInputMappingContext.generated.h"

UCLASS()
class UCCTVInputMappingContext : public UInputMappingContext
{
	GENERATED_BODY()

public:
	FEnhancedActionKeyMapping InteractMapping;

	UCCTVInputMappingContext();
	FEnhancedActionKeyMapping& GetInteractKeyMapping();
	void PostInitProperties();
};
