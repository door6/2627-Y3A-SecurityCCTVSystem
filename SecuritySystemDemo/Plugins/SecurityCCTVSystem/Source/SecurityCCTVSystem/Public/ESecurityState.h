// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ESecurityState.generated.h"

UENUM(BlueprintType)
enum class ESecurityState : uint8
{
    Neutral UMETA(DisplayName = "Neutral"),
    Alarm UMETA(DisplayName = "Alarm")  
};
