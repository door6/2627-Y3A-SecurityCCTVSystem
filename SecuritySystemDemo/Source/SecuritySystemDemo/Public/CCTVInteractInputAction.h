// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "CCTVInteractInputAction.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UCCTVInteractInputAction : public UInputAction
{
	GENERATED_BODY()
	
	UCCTVInteractInputAction()
	{
		ValueType = EInputActionValueType::Boolean;
	};
};

UCLASS(Blueprintable, BlueprintType)
class UCCTVNextCamInputAction : public UInputAction
{
	GENERATED_BODY()

	UCCTVNextCamInputAction()
	{
		ValueType = EInputActionValueType::Boolean;
	};
};

UCLASS(Blueprintable, BlueprintType)
class UCCTVPrevCamInputAction : public UInputAction
{
	GENERATED_BODY()

	UCCTVPrevCamInputAction()
	{
		ValueType = EInputActionValueType::Boolean;
	};
};

UCLASS(Blueprintable, BlueprintType)
class UCCTVTriggerInputAction : public UInputAction
{
	GENERATED_BODY()

	UCCTVTriggerInputAction()
	{
		ValueType = EInputActionValueType::Boolean;
	};
};

UCLASS(Blueprintable, BlueprintType)
class UCCTVExitInputAction : public UInputAction
{
	GENERATED_BODY()

	UCCTVExitInputAction()
	{
		ValueType = EInputActionValueType::Boolean;
	};
};