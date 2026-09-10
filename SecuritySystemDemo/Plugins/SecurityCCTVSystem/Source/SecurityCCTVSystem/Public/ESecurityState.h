// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/Class.h"
#include "ESecurityState.generated.h"

/**
 * 

UCLASS()
class SECURITYCCTVSYSTEM_API UESecurityState : public UEnum
{
	GENERATED_BODY()

public:
    enum
    {
        Alarm,
        Neutral
    };
	
};
 */



/**
 * Custom enum to demonstrate how to expose enums to Blueprints.
 */
UENUM(BlueprintType)
enum class ESecurityState : uint8
{
    Alarm UMETA(DisplayName = "Alarm"),
    Neutral UMETA(DisplayName = "Neutral")
};
