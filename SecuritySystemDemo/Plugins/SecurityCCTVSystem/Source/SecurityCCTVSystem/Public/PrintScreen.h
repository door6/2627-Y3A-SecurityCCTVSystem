// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PrintScreen.generated.h"

/**
 * 
 */
UCLASS()
class SECURITYCCTVSYSTEM_API UPrintScreen : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void PrintScreen(const FString& String, const FColor Color = FColor::Green, const float Time = 5.f);
	
};
