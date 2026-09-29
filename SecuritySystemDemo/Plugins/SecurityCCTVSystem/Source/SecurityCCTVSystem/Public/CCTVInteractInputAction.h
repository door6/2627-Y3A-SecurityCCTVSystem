// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "CCTVInteractInputAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SECURITYCCTVSYSTEM_API UCCTVInteractInputAction : public UInputAction
{
	GENERATED_BODY()
	
	UCCTVInteractInputAction()
	{
		ValueType = EInputActionValueType::Boolean;
	};
};
